#include "ir.h"
#include "ast.h"

namespace PascalAST
{

    std::shared_ptr<PascalBack::Register> ASTNode::GenIR(SymbolTable &table, PascalBack::IRTable &irTable) {}

    std::shared_ptr<PascalBack::Register> ExpressionFactor::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        return expression->GenIR(table, irTable);
    }

    std::shared_ptr<PascalBack::Register> Variable::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        bool has;
        int layer;
        auto &type = table.FindSymbol(name, has, layer)->second.type;
        auto targetType(((WrapperType *)type.get())->DeWrap());
        if (varPart != nullptr)
        {
            if (varPart->isProcedureCall)
            {
            }
            else
            {
                //TODO
                if (isAssignLeft)
                {
                                }
                else
                {
                }
            }
        }
        else
        {
            if (targetType->GetTypeId() == FUNC)
            {
                return irTable.InsertCallInstr(name, std::vector<std::shared_ptr<PascalBack::Register>>(), std::vector<std::string>());
            }
            else
            {
                return irTable.GetNamedRegister(name);
            }
        }
    }
    std::shared_ptr<PascalBack::Register> VariableList::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
    }

    std::shared_ptr<PascalBack::Register> NumFactor::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        return std::make_shared<PascalBack::Register>(PascalBack::GenRegTypeByStr(type), true, val);
    }

    std::shared_ptr<PascalBack::Register> InvFactor::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        auto reg2 = subFactor->GenIR(table, irTable);
        auto reg1 = std::make_shared<PascalBack::Register>(reg2->type, true, "0");
        return irTable.InsertBasicInstr(reg1, reg2, PascalBack::IROpCode::SUB);
    }

    std::shared_ptr<PascalBack::Register> VariableFactor::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        return variable->GenIR(table, irTable);
    }

    std::shared_ptr<PascalBack::Register> NotFactor::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        auto reg2 = subFactor->GenIR(table, irTable);
        auto reg1 = std::make_shared<PascalBack::Register>(reg2->type, true, "0");
        return irTable.InsertBasicInstr(reg1, reg2, PascalBack::IROpCode::AND);
    }

    std::shared_ptr<PascalBack::Register> MulOpPart::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        auto reg1 = secondFactor->GenIR(table, irTable);
        if (followPart == nullptr)
            return reg1;
        auto reg2 = followPart->GenIR(table, irTable);
        return irTable.InsertBasicInstr(reg1, reg2, PascalBack::String2Op(followPart->mulOp));
    }
    std::shared_ptr<PascalBack::Register> Term::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        auto reg1 = firstFactor->GenIR(table, irTable);
        if (mulOpPart == nullptr)
            return reg1;
        auto reg2 = mulOpPart->GenIR(table, irTable);
        return irTable.InsertBasicInstr(reg1, reg2, PascalBack::String2Op(mulOpPart->mulOp));
    }

    std::shared_ptr<PascalBack::Register> AddOpPart::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        auto reg1 = secondTerm->GenIR(table, irTable);
        if (followPart == nullptr)
            return reg1;
        auto reg2 = followPart->GenIR(table, irTable);
        return irTable.InsertBasicInstr(reg1, reg2, PascalBack::String2Op(followPart->addOp));
    }

    std::shared_ptr<PascalBack::Register> SimpleExpression::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        auto reg1 = firstTerm->GenIR(table, irTable);
        if (addOpPart == nullptr)
            return reg1;
        auto reg2 = addOpPart->GenIR(table, irTable);
        return irTable.InsertBasicInstr(reg1, reg2, PascalBack::String2Op(addOpPart->addOp));
    }

    std::shared_ptr<PascalBack::Register> RelPart::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        return secondExpression->GenIR(table, irTable);
    }

    std::shared_ptr<PascalBack::Register> Expression::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        auto reg1 = firstExpression->GenIR(table, irTable);
        if (relPart == nullptr)
            return reg1;
        auto reg2 = relPart->GenIR(table, irTable);
        return irTable.InsertBasicInstr(reg1, reg2, PascalBack::String2Op(relPart->relop));
    }

    std::shared_ptr<PascalBack::Register> ExpressionList::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        for (auto &expression : expressions)
            expression->GenIR(table, irTable);
        return std::shared_ptr<PascalBack::Register>();
    }

    std::shared_ptr<PascalBack::Register> VarPart::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        //TODO
        return std::shared_ptr<PascalBack::Register>();
    }
    std::shared_ptr<PascalBack::Register> VariableAssignStatement::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        //TODO
        return std::shared_ptr<PascalBack::Register>();
    }
    std::shared_ptr<PascalBack::Register> ProcedureCallStatement::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        return std::shared_ptr<PascalBack::Register>();
    }
    std::shared_ptr<PascalBack::Register> SubCompoundStatement::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        compoundStatement->GenIR(table, irTable);
        return std::shared_ptr<PascalBack::Register>();
    }
    std::shared_ptr<PascalBack::Register> IfElseStatement::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        return std::shared_ptr<PascalBack::Register>();
    }
    std::shared_ptr<PascalBack::Register> ForLoopStatement::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        return std::shared_ptr<PascalBack::Register>();
    }
    std::shared_ptr<PascalBack::Register> ReadStatement::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        return std::shared_ptr<PascalBack::Register>();
    }
    std::shared_ptr<PascalBack::Register> WriteStatement::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        return std::shared_ptr<PascalBack::Register>();
    }
    std::shared_ptr<PascalBack::Register> StatementList::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        for (auto &statement : statements)
            statement->GenIR(table, irTable);
        return std::shared_ptr<PascalBack::Register>();
    }
    std::shared_ptr<PascalBack::Register> CompoundStatement::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        statementList->GenIR(table, irTable);
        return std::shared_ptr<PascalBack::Register>();
    }
    std::shared_ptr<PascalBack::Register> SubProgramHead::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        return std::shared_ptr<PascalBack::Register>();
    }
    std::shared_ptr<PascalBack::Register> SubProgramBody::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        compoundStatement->GenIR(table, irTable);
        return std::shared_ptr<PascalBack::Register>();
    }
    std::shared_ptr<PascalBack::Register> SubProgram::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        head->GenIR(table, irTable);
        body->GenIR(table, irTable);
        return std::shared_ptr<PascalBack::Register>();
    }
    std::shared_ptr<PascalBack::Register> SubProgramDeclarations::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        for (auto &subProgram : subPrograms)
            subProgram->GenIR(table, irTable);
        return std::shared_ptr<PascalBack::Register>();
    }
    std::shared_ptr<PascalBack::Register> ProgramBody::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        subProgramDeclarations->GenIR(table, irTable);
        compoundStatemnet->GenIR(table, irTable);
        return std::shared_ptr<PascalBack::Register>();
    }
    std::shared_ptr<PascalBack::Register> Program::GenIR(SymbolTable &table, PascalBack::IRTable &irTable)
    {
        programBody->GenIR(table, irTable);
        return std::shared_ptr<PascalBack::Register>();
    }
}