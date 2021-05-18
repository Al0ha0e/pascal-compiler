#ifndef IR
#define IR

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <set>
#include <list>

namespace PascalBack
{
    enum DataType
    {
        INTEGER,
        FLOAT
    };

    typedef std::pair<DataType, int> RegType;

    enum IROpCode
    {
        ADD,
        SUB,
        AND,
        OR,
        MUL,
        DIV,
        MOD,
        EQ,
        NE,
        GT,
        LT,
        GE,
        LE
    };

    struct TriInstruction
    {
    };

    struct Register
    {
        RegType type;
        bool isConstant;
        std::string val;
        std::shared_ptr<TriInstruction> from;
        std::vector<std::shared_ptr<TriInstruction>> to; //duplicate edge

        Register() {}
        Register(RegType type,
                 bool isConstant,
                 std::string val = "",
                 std::shared_ptr<TriInstruction> from = std::shared_ptr<TriInstruction>())
            : type(type), isConstant(isConstant), val(val), from(from) {}
    };

    struct MemoryBlock
    {
        int size;
    };

    struct VariableInfo
    {
        std::vector<std::shared_ptr<Register>> namedRegs;
        bool isRef;
        bool changed;
    };

    struct BasicBlock
    {
        int id;
        std::vector<int> prevBlocks;
        std::vector<int> nxtBlocks;
        std::map<std::string, VariableInfo> localVarInfo;
        std::map<std::string, VariableInfo> globalVarInfo;
        std::list<std::shared_ptr<TriInstruction>> instrs;

        void pushInstr(std::shared_ptr<TriInstruction> instr)
        {
            instrs.push_back(instr);
        }

        bool HasNamedRegister(std::string name)
        {
            return localVarInfo.find(name) != localVarInfo.end();
        }

        std::shared_ptr<Register> GetNamedRegister(std::string name)
        {
            auto varInfo = localVarInfo.find(name);
            if (varInfo == localVarInfo.end())
                varInfo = globalVarInfo.find(name);

            auto &regs = varInfo->second.namedRegs;
            return regs[regs.size() - 1];
        }

        void InsertNamedReg(std::string name, std::shared_ptr<Register> reg)
        {
            auto varInfo = localVarInfo.find(name);
            if (varInfo == localVarInfo.end())
                varInfo = globalVarInfo.find(name);
            varInfo->second.changed = true;
            varInfo->second.namedRegs.push_back(reg);
        }
    };

    struct Function
    {
        std::string name;
        std::map<std::string, std::shared_ptr<MemoryBlock>> namedMemory;
        std::set<std::pair<int, std::string>> prevBlocks;
        std::map<int, std::shared_ptr<BasicBlock>> blockMap;
        std::list<std::shared_ptr<BasicBlock>> basicBlocks;

        std::shared_ptr<Register> GenRetReg()
        {
            if (!(*basicBlocks.begin())->HasNamedRegister(name + "_ret"))
                return std::shared_ptr<Register>();
            auto retReg = (*basicBlocks.begin())->GetNamedRegister(name + "_ret");
            return std::make_shared<Register>(retReg->type, false);
        }
    };

    struct Program
    {
        int blockNum;
        std::map<std::string, VariableInfo> varInfo;
        std::map<std::string, std::shared_ptr<MemoryBlock>> namedMemory;
        std::map<std::string, std::shared_ptr<Function>> functions;

        std::shared_ptr<Function> GetFunction(std::string name)
        {
            return functions.find(name)->second;
        }
    };

    struct BasicInstruction : public TriInstruction
    {
        IROpCode opcode;
        std::shared_ptr<Register> reg1;
        std::shared_ptr<Register> reg2;
        std::shared_ptr<Register> ret;

        BasicInstruction() {}
        BasicInstruction(IROpCode opcode,
                         std::shared_ptr<Register> reg1,
                         std::shared_ptr<Register> reg2,
                         std::shared_ptr<Register> ret)
            : opcode(opcode), reg1(reg1), reg2(reg2), ret(ret)
        {
            ret->from = std::shared_ptr<TriInstruction>((TriInstruction *)this);
        }
    };

    struct PhiInstruction : public TriInstruction
    {
        std::vector<std::shared_ptr<Register>> refRegisters;
        std::shared_ptr<Register> ret;

        PhiInstruction() {}
        PhiInstruction(std::vector<std::shared_ptr<Register>> &&refRegisters, std::shared_ptr<Register> ret)
            : refRegisters(refRegisters), ret(ret)
        {
            ret->from = std::shared_ptr<TriInstruction>((TriInstruction *)this);
        }
    };

    struct MemoryInstruction : public TriInstruction
    {
        bool isLoad;
        std::shared_ptr<Register> addr; //offset relative to start of the memory block
        std::shared_ptr<Register> val;
        std::shared_ptr<MemoryBlock> memory;
    };

    struct JumpInstruction : public TriInstruction
    {
        std::shared_ptr<Register> cond;
        std::shared_ptr<BasicBlock> succBlock;
        std::shared_ptr<BasicBlock> failBlock;
    };

    struct CallInstruction : public TriInstruction
    {
        // std::vector<std::shared_ptr<>>
        std::vector<std::shared_ptr<Register>> argRegs;
        std::vector<std::shared_ptr<Register>> refNamedRegs;
        std::shared_ptr<Register> ret;
        std::shared_ptr<Function> targetFunc;

        CallInstruction() {}
        CallInstruction(std::vector<std::shared_ptr<Register>> &&argRegs,
                        std::vector<std::shared_ptr<Register>> &&refNamedRegs,
                        std::shared_ptr<Register> ret,
                        std::shared_ptr<Function> targetFunc)
            : argRegs(argRegs), refNamedRegs(refNamedRegs), ret(ret), targetFunc(targetFunc)
        {
            ret->from = std::shared_ptr<TriInstruction>((TriInstruction *)this);
            for (auto &refReg : refNamedRegs)
                refReg->from = ret->from;
        }
    };

    struct IRTable
    {
        std::shared_ptr<Program> curProgram;
        std::shared_ptr<Function> curFunction;
        std::shared_ptr<BasicBlock> curBlock;

        std::shared_ptr<Register> InsertBasicInstr(std::shared_ptr<Register> reg1, std::shared_ptr<Register> reg2, IROpCode op);

        std::shared_ptr<Register> InsertCallInstr(
            std::string funcName,
            std::vector<std::shared_ptr<Register>> &&args,
            std::vector<std::string> &&refNames);

        std::shared_ptr<Register> GetNamedRegister(std::string name);
    };

    RegType GenRegTypeByStr(std::string s)
    {
        if (s == "integer" || s == "int")
        {
            return RegType(INTEGER, 4);
        }
        if (s == "real" || s == "float")
        {
            return RegType(FLOAT, 4);
        }
        if (s == "char")
        {
            return RegType(INTEGER, 1);
        }
        if (s == "boolean")
        {
            return RegType(INTEGER, 1);
        }
        return RegType(INTEGER, 1);
    }

    inline IROpCode String2Op(std::string s)
    {
        if (s == "div")
            return IROpCode::DIV;
        if (s == "and")
            return IROpCode::AND;
        if (s == "mod")
            return IROpCode::MOD;
        if (s == "*")
            return IROpCode::MUL;
        if (s == "/")
            return IROpCode::DIV;
        if (s == "or")
            return IROpCode::OR;
        if (s == "+")
            return IROpCode::ADD;
        if (s == "-")
            return IROpCode::SUB;
        if (s == "=")
            return IROpCode::EQ;
        if (s == "<>")
            return IROpCode::NE;
        if (s == "<")
            return IROpCode::LT;
        if (s == ">")
            return IROpCode::GT;
        if (s == "<=")
            return IROpCode::LE;
        if (s == ">=")
            return IROpCode::GE;
    }
    // std::shared_ptr<Register> GenConstRegister(RegType type, std::string val)
    // {
    //     auto reg = new PascalBack::Register(type, true, val);
    //     return std::shared_ptr<PascalBack::Register>((PascalBack::Register *)reg);
    // }

    // std::shared_ptr<Register> GenRegister(RegType type)
    // {
    //     auto reg = new PascalBack::Register(type, false, "");
    //     return std::shared_ptr<PascalBack::Register>((PascalBack::Register *)reg);
    // }

}

#endif