#ifndef IR
#define IR

#include <string>
#include <memory>
#include <vector>

namespace PascalBack
{
    enum DataType
    {
        INTEGER,
        FLOAT
    };

    enum IROpCode
    {
    };

    struct Register;
    struct BasicBlock;

    struct TriInstruction
    {
    };

    struct BasicInstruction : public TriInstruction
    {
        IROpCode opcode;
        std::shared_ptr<Register> op1;
        std::shared_ptr<Register> op2;
        std::shared_ptr<Register> ret;
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
        std::shared_ptr<BasicBlock> nextBlock;
    };

    struct Register
    {
        DataType type;
        int size;
        bool isConstant;
        std::string val;
        std::shared_ptr<TriInstruction> from;
        std::vector<std::shared_ptr<TriInstruction>> to; //duplicate edge
    };

    struct BasicBlock
    {
        int id;
        std::map<int, std::shared_ptr<BasicBlock>> prevBlocks;
        std::vector<std::shared_ptr<TriInstruction>> instrs;
    };

    struct VariableInfo
    {
        std::vector<std::shared_ptr<Register>> namedRegs;
        bool isRef;
    };

    struct Function
    {
        int stackId;
        std::map<std::string, VariableInfo> varInfo;
        std::shared_ptr<BasicBlock> firstBlock;
    };
}

#endif