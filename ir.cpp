#include "ir.h"

namespace PascalBack
{
    std::shared_ptr<Register> IRTable::InsertBasicInstr(std::shared_ptr<Register> reg1, std::shared_ptr<Register> reg2, IROpCode op)
    {
        auto type = reg1->type;
        //TODO TYPE CONV
        auto ret = std::make_shared<Register>(type, false);
        auto instr = new BasicInstruction(op, reg1, reg2, ret);
        curBlock->pushInstr(std::shared_ptr<TriInstruction>((TriInstruction *)instr));
        return ret;
    }

    std::shared_ptr<Register> IRTable::InsertCallInstr(
        std::string funcName,
        std::vector<std::shared_ptr<Register>> &&args,
        std::vector<std::string> &&refNames,
        std::string retType)
    {
        std::vector<std::shared_ptr<Register>> refRegs;
        for (auto name : refNames)
        {
            auto refReg = std::make_shared<Register>(); //TODO: TYPE
            curBlock->InsertNamedReg(name, refReg);
            refRegs.push_back(refReg);
        }

        std::shared_ptr<Register> ret;
        if (retType != "void")
            ret = std::make_shared<Register>(GenRegTypeByStr(retType), false);

        auto instr = new CallInstruction(std::move(args), std::move(refRegs), ret, curProgram->GetFunction(funcName));
        curBlock->pushInstr(std::shared_ptr<TriInstruction>((TriInstruction *)instr));
    }

    std::shared_ptr<Register> IRTable::GetNamedRegister(std::string name)
    {
        return curBlock->GetNamedRegister(name);
    }
}
