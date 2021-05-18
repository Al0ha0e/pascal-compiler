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
        std::vector<std::string> &&refNames)
    {
        std::vector<std::shared_ptr<Register>> refRegs;
        for (auto name : refNames)
        {
            auto type = curBlock->GetNamedRegister(name)->type;
            auto refReg = std::make_shared<Register>(type, false);
            curBlock->InsertNamedReg(name, refReg);
            refRegs.push_back(refReg);
        }
        auto func = curProgram->GetFunction(funcName);
        std::shared_ptr<Register> ret = func->GenRetReg();

        auto instr = new CallInstruction(std::move(args), std::move(refRegs), ret, func);
        curBlock->pushInstr(std::shared_ptr<TriInstruction>((TriInstruction *)instr));
    }

    std::shared_ptr<Register> IRTable::GetNamedRegister(std::string name)
    {
        return curBlock->GetNamedRegister(name);
    }
}
