#include "types.h"
#include <iostream>

namespace PascalAST
{
    std::unique_ptr<TypeInfo> TypeInfo::CalcType(std::unique_ptr<TypeInfo> &&anotherType, std::string op, bool &ok, std::string &errMsg)
    {
        ok = false;
        errMsg = std::string("operation ") + op + " is not compatiable with type " + ToString();
        if (anotherType->IsBasicType())
        {
            return std::move(anotherType);
        }
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> TypeInfo::CalcFuncType(std::unique_ptr<TupleType> &&argTypes, bool &ok, std::string &errMsg)
    {
        ok = false;
        errMsg = std::string("type ") + ToString() + " is not callable";
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> TypeInfo::CalcArrayType(std::unique_ptr<TupleType> &&idTypes, bool &ok, std::string &errMsg)
    {
        ok = false;
        errMsg = std::string("type ") + ToString() + " is not subscriptable";
        return GenType(VOID);
    }

    //VOIDType
    std::unique_ptr<TypeInfo> VOIDType::Copy()
    {
        TypeInfo *ret = new VOIDType();
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::string VOIDType::ToString()
    {
        return "void";
    }

    bool VOIDType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        errMsg = std::string("type ") + ToString() + " cannot be initialized by type " + anotherType->ToString();
        return false;
    }

    bool VOIDType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        errMsg = std::string("type ") + ToString() + " cannot be assigned by type" + anotherType->ToString();
        return false;
    }

    //IntegerType

    std::unique_ptr<TypeInfo> IntegerType::Copy()
    {
        TypeInfo *ret = new IntegerType();
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::string IntegerType::ToString()
    {
        return "integer";
    }

    bool IntegerType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        std::string anoTypeStr = anotherType->ToString();
        if (!AssignCompatible(std::move(anotherType), errMsg))
        {
            errMsg = std::string("type ") + ToString() + " cannot be initialized by type " + anoTypeStr;
            return false;
        }
        return true;
    }
    bool IntegerType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        if (anotherType->IsBasicType())
            return true;
        errMsg = std::string("type ") + ToString() + " cannot be assigned by type" + anotherType->ToString();
        return false;
    }

    std::unique_ptr<TypeInfo> IntegerType::CalcType(std::unique_ptr<TypeInfo> &&anotherType, std::string op, bool &ok, std::string &errMsg)
    {
        if (op == "/" && anotherType->IsBasicType())
            return GenType(REAL);

        if (anotherType->GetTypeId() == REAL)
        {
            if (op == "div")
            {
                ok = false;
                errMsg = std::string("operation ") + op + " is not compatiable with type " + anotherType->ToString();
                return GenType(VOID);
            }
            return std::move(anotherType);
        }
        if (anotherType->IsBasicType())
            return GenType(INTEGER);

        ok = false;
        errMsg = std::string("operation ") + op + " is not compatiable with type " + anotherType->ToString();
        return GenType(VOID);
    }

    //RealType

    std::unique_ptr<TypeInfo> RealType::Copy()
    {
        TypeInfo *ret = new RealType();
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::string RealType::ToString()
    {
        return "real";
    }

    bool RealType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        std::string anoTypeStr = anotherType->ToString();
        if (!AssignCompatible(std::move(anotherType), errMsg))
        {
            errMsg = std::string("type ") + ToString() + " cannot be initialized by type " + anoTypeStr;
            return false;
        }
        return true;
    }
    bool RealType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        if (anotherType->IsBasicType())
            return true;
        errMsg = std::string("type ") + ToString() + " cannot be assigned by type" + anotherType->ToString();
        return false;
    }

    std::unique_ptr<TypeInfo> RealType::CalcType(std::unique_ptr<TypeInfo> &&anotherType, std::string op, bool &ok, std::string &errMsg)
    {
        if (op == "div")
        {
            ok = false;
            errMsg = std::string("operation ") + op + " is not compatiable with type " + ToString();
            return GenType(VOID);
        }
        if (anotherType->IsBasicType())
            return GenType(REAL);

        ok = false;
        errMsg = std::string("operation ") + op + " is not compatiable with type " + anotherType->ToString();
        return GenType(VOID);
    }

    //CharType

    std::unique_ptr<TypeInfo> CharType::Copy()
    {
        TypeInfo *ret = new CharType();
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::string CharType::ToString()
    {
        return "char";
    }

    bool CharType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        std::string anoTypeStr = anotherType->ToString();
        if (!AssignCompatible(std::move(anotherType), errMsg))
        {
            errMsg = std::string("type ") + ToString() + " cannot be initialized by type " + anoTypeStr;
            return false;
        }
        return true;
    }

    bool CharType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        if (anotherType->IsBasicType())
            return true;
        errMsg = std::string("type ") + ToString() + " cannot be assigned by type" + anotherType->ToString();
        return false;
    }

    std::unique_ptr<TypeInfo> CharType::CalcType(std::unique_ptr<TypeInfo> &&anotherType, std::string op, bool &ok, std::string &errMsg)
    {
        if (op == "/" && anotherType->IsBasicType())
            return GenType(REAL);

        if (op == "div" && anotherType->GetTypeId() == REAL)
        {
            ok = false;
            errMsg = std::string("operation ") + op + " is not compatiable with type " + anotherType->ToString();
            return GenType(VOID);
        }

        if (anotherType->GetTypeId() == BOOLEAN)
            return GenType(CHAR);

        if (anotherType->IsBasicType())
            return std::move(anotherType);

        ok = false;
        errMsg = std::string("operation ") + op + " is not compatiable with type " + anotherType->ToString();
        return GenType(VOID);
    }

    //BooleanType

    std::unique_ptr<TypeInfo> BooleanType::Copy()
    {
        TypeInfo *ret = new BooleanType();
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::string BooleanType::ToString()
    {
        return "boolean";
    }

    bool BooleanType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        std::string anoTypeStr = anotherType->ToString();
        if (!AssignCompatible(std::move(anotherType), errMsg))
        {
            errMsg = std::string("type ") + ToString() + " cannot be initialized by type " + anoTypeStr;
            return false;
        }
        return true;
    }

    bool BooleanType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        if (anotherType->IsBasicType())
            return true;
        errMsg = std::string("type ") + ToString() + " cannot be assigned by type" + anotherType->ToString();
        return false;
    }

    std::unique_ptr<TypeInfo> BooleanType::CalcType(std::unique_ptr<TypeInfo> &&anotherType, std::string op, bool &ok, std::string &errMsg)
    {
        if (op == "/" && anotherType->IsBasicType())
            return GenType(REAL);

        if (op == "div" && anotherType->GetTypeId() == REAL)
        {
            ok = false;
            errMsg = std::string("operation ") + op + " is not compatiable with type " + anotherType->ToString();
            return GenType(VOID);
        }
        if (anotherType->IsBasicType())
            return std::move(anotherType);

        ok = false;
        errMsg = std::string("operation ") + op + " is not compatiable with type " + anotherType->ToString();
        return GenType(VOID);
    }

    //TupleType

    std::unique_ptr<TypeInfo> TupleType::Copy()
    {
        TypeInfo *ret = new TupleType(subTypes);
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::string TupleType::ToString()
    {
        std::string ret = "Tuple(";
        for (auto &subType : subTypes)
        {
            ret += subType->ToString() + ",";
        }
        ret += ")";
        return ret;
    }

    bool TupleType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        if (anotherType->GetTypeId() != TUPLE)
        {
            errMsg = std::string("type ") + ToString() + " cannot be initialized by type " + anotherType->ToString();
            return false;
        }
        std::string anoTypeStr = anotherType->ToString();
        auto ano(UniquePtrCast<TupleType>(anotherType));
        if (ano->subTypes.size() != subTypes.size())
        {
            errMsg = std::string("type ") + ToString() + " cannot be initialized by type " + anoTypeStr;
            return false;
        }

        bool compatible = true;
        for (int i = 0; i < subTypes.size(); i++)
        {
            if (!subTypes[i]->InitCompatible(std::move(ano->subTypes[i]), errMsg))
            {
                compatible = false;
                errMsg = std::string("type ") + ToString() + " cannot be initialized by type " + anoTypeStr;
                break;
            }
        }
        return compatible;
    }

    bool TupleType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        if (anotherType->GetTypeId() != TUPLE)
        {
            errMsg = std::string("type ") + ToString() + " cannot be assigned by type" + anotherType->ToString();
            return false;
        }
        std::string anoTypeStr = anotherType->ToString();
        auto ano(UniquePtrCast<TupleType>(anotherType));
        if (ano->subTypes.size() != subTypes.size())
        {
            errMsg = std::string("type ") + ToString() + " cannot be assigned by type" + anoTypeStr;
            return false;
        }

        bool compatible = true;
        for (int i = 0; i < subTypes.size(); i++)
        {
            if (!subTypes[i]->AssignCompatible(std::move(ano->subTypes[i]), errMsg))
            {
                compatible = false;
                errMsg = std::string("type ") + ToString() + " cannot be initialized by type " + anoTypeStr;
                break;
            }
        }
        return compatible;
    }

    //FuncType

    std::unique_ptr<TypeInfo> FuncType::Copy()
    {
        TypeInfo *ret = new FuncType(UniquePtrCast<TupleType>(argTypes->Copy()), retType->Copy());
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::string FuncType::ToString()
    {
        std::string ret = argTypes->ToString() + "->" + retType->ToString();
        return ret;
    }

    bool FuncType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        errMsg = std::string("type ") + ToString() + " cannot be initialized by type " + anotherType->ToString();
        return false;
    }

    bool FuncType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        errMsg = std::string("type ") + ToString() + " cannot be assigned by type" + anotherType->ToString();
        return false;
    }

    std::unique_ptr<TypeInfo> FuncType::CalcFuncType(std::unique_ptr<TupleType> &&argTypes, bool &ok, std::string &errMsg)
    {
        if (!this->argTypes->InitCompatible(std::move(argTypes), errMsg))
        {
            ok = false;
            errMsg = std::string("Function args not compatible: ") + errMsg;
        }

        return retType->Copy();
    }

    //ArrayType

    std::unique_ptr<TypeInfo> ArrayType::Copy()
    {
        TypeInfo *ret = new ArrayType(dimensions, contentType->Copy());
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::string ArrayType::ToString()
    {
        std::string ret = contentType->ToString() + "[";
        for (auto &dim : dimensions)
            ret += std::to_string(dim.first) + ".." + std::to_string(dim.second) + ",";
        return ret + "]";
    }

    bool ArrayType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        errMsg = std::string("type ") + ToString() + " cannot be initialized by type " + anotherType->ToString();
        return false;
    }

    bool ArrayType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        errMsg = std::string("type ") + ToString() + " cannot be assigned by type" + anotherType->ToString();
        return false;
    }

    std::unique_ptr<TypeInfo> ArrayType::CalcArrayType(std::unique_ptr<TupleType> &&idTypes, bool &ok, std::string &errMsg)
    {
        auto subTypes(idTypes->GetSubTypes());
        if (subTypes.size() != dimensions.size())
        {
            ok = false;
            errMsg = std::string("Subscript dimension mismatch, met: ") + std::to_string(subTypes.size()) + " expecting: " + std::to_string(dimensions.size());
        }
        else
        {
            for (auto &type : subTypes)
            {
                auto targetType = ((WrapperType *)type.get())->DeWrap();
                if ((!targetType->IsBasicType()) || (targetType->GetTypeId() == REAL))
                {
                    ok = false;
                    errMsg = std::string("Subscript type must be integer");
                    break;
                }
            }
        }

        return contentType->Copy();
    }

    //WrapperType
    std::unique_ptr<TypeInfo> WrapperType::CalcType(std::unique_ptr<TypeInfo> &&anotherType, std::string op, bool &ok, std::string &errMsg)
    {
        if (anotherType->IsWrapperType())
            anotherType = UniquePtrCast<WrapperType>(anotherType)->DeWrap();
        TypeInfo *ret = new RValueType(targetType->CalcType(std::move(anotherType), op, ok, errMsg));
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> WrapperType::CalcFuncType(std::unique_ptr<TupleType> &&argTypes, bool &ok, std::string &errMsg)
    {
        TypeInfo *ret = new RValueType(targetType->CalcFuncType(std::move(argTypes), ok, errMsg));
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> WrapperType::CalcArrayType(std::unique_ptr<TupleType> &&idTypes, bool &ok, std::string &errMsg)
    {
        TypeInfo *ret = new LValueType(targetType->CalcArrayType(std::move(idTypes), ok, errMsg));
        return std::unique_ptr<TypeInfo>(ret);
    }

    //LValueType
    std::unique_ptr<TypeInfo> LValueType::Copy()
    {
        TypeInfo *lValueType = new LValueType(targetType->Copy());
        return std::unique_ptr<TypeInfo>(lValueType);
    }

    std::string LValueType::ToString()
    {
        std::string ret = "LVal(";
        ret += targetType->ToString();
        return ret + ")";
    }

    bool LValueType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        std::string anoTypeStr = anotherType->ToString();
        bool compatible = AssignCompatible(std::move(anotherType), errMsg);
        if (!compatible)
            errMsg = std::string("type ") + ToString() + " cannot be initialized by type " + anoTypeStr;
        return compatible;
    }

    bool LValueType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        bool compatible;
        std::string anoTypeStr = anotherType->ToString();

        if (anotherType->IsWrapperType())
        {
            auto ano(UniquePtrCast<WrapperType>(anotherType)->DeWrap());
            compatible = targetType->AssignCompatible(std::move(ano), errMsg);
        }
        else
        {
            compatible = targetType->AssignCompatible(std::move(anotherType), errMsg);
        }
        if (!compatible)
            errMsg = std::string("type ") + ToString() + " cannot be assigned by type" + anoTypeStr;
        return compatible;
    }

    //RValueType
    std::unique_ptr<TypeInfo> RValueType::Copy()
    {
        TypeInfo *rValueType = new RValueType(targetType->Copy());
        return std::unique_ptr<TypeInfo>(rValueType);
    }

    std::string RValueType::ToString()
    {
        std::string ret = "RVal(";
        ret += targetType->ToString();
        return ret + ")";
    }

    bool RValueType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        errMsg = std::string("type ") + ToString() + " cannot be initialized by type " + anotherType->ToString();
        return false;
    }

    bool RValueType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        errMsg = std::string("type ") + ToString() + " cannot be assigned by type" + anotherType->ToString();
        return false;
    }

    //RefType
    std::unique_ptr<TypeInfo> RefType::Copy()
    {
        TypeInfo *refValueType = new RefType(targetType->Copy());
        return std::unique_ptr<TypeInfo>(refValueType);
    }

    std::string RefType::ToString()
    {
        std::string ret = "Ref(";
        ret += targetType->ToString();
        return ret + ")";
    }

    bool RefType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        TypeID tid = anotherType->GetTypeId();
        std::string anoTypeStr = anotherType->ToString();
        bool compatible;

        if (tid == REF || tid == LVALUE)
        {
            auto ano(UniquePtrCast<WrapperType>(anotherType)->DeWrap());
            compatible = targetType->InitCompatible(std::move(ano), errMsg);
        }
        else
        {
            compatible = false;
        }
        if (!compatible)
            errMsg = std::string("type ") + ToString() + " cannot be initialized by type " + anoTypeStr;
        return compatible;
    }

    bool RefType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg)
    {
        std::string anoTypeStr = anotherType->ToString();
        bool compatiable;

        if (anotherType->IsWrapperType())
        {
            auto ano(UniquePtrCast<WrapperType>(anotherType)->DeWrap());
            compatiable = targetType->AssignCompatible(std::move(ano), errMsg);
        }
        else
        {
            compatiable = targetType->AssignCompatible(std::move(anotherType), errMsg);
        }
        if (!compatiable)
            errMsg = std::string("type ") + ToString() + " cannot be assigned by type" + anoTypeStr;
        return compatiable;
    }
}