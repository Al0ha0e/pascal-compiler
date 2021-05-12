#include "types.h"
#include <iostream>

namespace PascalAST
{
    //TODO: Error Handling
    std::unique_ptr<TypeInfo> TypeInfo::CalcType(std::unique_ptr<TypeInfo> &&anotherType, bool &ok)
    {
        ok = false;
        std::cout << "$$$$$VOIDTYPE CNNOT CALC" << std::endl;
        if (anotherType->IsBasicType())
        {
            return std::move(anotherType);
        }
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> TypeInfo::CalcFuncType(std::unique_ptr<TupleType> &&argTypes, bool &ok)
    {
        ok = false;
        std::cout << "$$$$$VOIDTYPE CNNOT CALL" << std::endl;
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> TypeInfo::CalcArrayType(std::unique_ptr<TupleType> &&idTypes, bool &ok)
    {
        ok = false;
        std::cout << "$$$$$VOIDTYPE CNNOT INDEX" << std::endl;
        return GenType(VOID);
    }

    //VOIDType
    std::unique_ptr<TypeInfo> VOIDType::Copy()
    {
        TypeInfo *ret = new VOIDType();
        return std::unique_ptr<TypeInfo>(ret);
    }

    bool VOIDType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        return false;
    }
    bool VOIDType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        return false;
    }

    //IntegerType

    std::unique_ptr<TypeInfo> IntegerType::Copy()
    {
        TypeInfo *ret = new IntegerType();
        return std::unique_ptr<TypeInfo>(ret);
    }

    bool IntegerType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        std::cout << "HAHAHA" << std::endl;
        return AssignCompatible(std::move(anotherType));
    }
    bool IntegerType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        if (anotherType->IsBasicType())
            return true;
        return false;
    }

    std::unique_ptr<TypeInfo> IntegerType::CalcType(std::unique_ptr<TypeInfo> &&anotherType, bool &ok)
    {
        if (anotherType->GetTypeId() == REAL)
        {
            return std::move(anotherType);
        }
        if (anotherType->IsBasicType())
        {
            return GenType(INTEGER);
        }
        return GenType(VOID);
    }

    //RealType

    std::unique_ptr<TypeInfo> RealType::Copy()
    {
        TypeInfo *ret = new RealType();
        return std::unique_ptr<TypeInfo>(ret);
    }

    bool RealType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        return AssignCompatible(std::move(anotherType));
    }
    bool RealType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        if (anotherType->IsBasicType())
            return true;
        return false;
    }

    std::unique_ptr<TypeInfo> RealType::CalcType(std::unique_ptr<TypeInfo> &&anotherType, bool &ok)
    {
        if (anotherType->IsBasicType())
        {
            return GenType(REAL);
        }
        return GenType(VOID);
    }

    //CharType

    std::unique_ptr<TypeInfo> CharType::Copy()
    {
        TypeInfo *ret = new CharType();
        return std::unique_ptr<TypeInfo>(ret);
    }
    bool CharType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        return AssignCompatible(std::move(anotherType));
    }
    bool CharType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        if (anotherType->IsBasicType())
            return true;
        return false;
    }

    std::unique_ptr<TypeInfo> CharType::CalcType(std::unique_ptr<TypeInfo> &&anotherType, bool &ok)
    {
        if (anotherType->IsBasicType())
        {
            return std::move(anotherType);
        }
        return GenType(VOID);
    }

    //BooleanType

    std::unique_ptr<TypeInfo> BooleanType::Copy()
    {
        TypeInfo *ret = new BooleanType();
        return std::unique_ptr<TypeInfo>(ret);
    }
    bool BooleanType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        return AssignCompatible(std::move(anotherType));
    }
    bool BooleanType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        if (anotherType->IsBasicType())
            return true;
        return false;
    }

    std::unique_ptr<TypeInfo> BooleanType::CalcType(std::unique_ptr<TypeInfo> &&anotherType, bool &ok)
    {
        if (anotherType->IsBasicType())
        {
            return std::move(anotherType);
        }
        return GenType(VOID);
    }

    //TupleType

    std::unique_ptr<TypeInfo> TupleType::Copy()
    {
        TypeInfo *ret = new TupleType(subTypes);
        return std::unique_ptr<TypeInfo>(ret);
    }

    bool TupleType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        if (anotherType->GetTypeId() != TUPLE)
        {
            std::cout << "$$$$$ANOTHER NOT TUPLE " << anotherType->GetTypeId() << std::endl;
            return false;
        }

        auto ano(UniquePtrCast<TupleType>(anotherType));
        if (ano->subTypes.size() != subTypes.size())
        {
            std::cout << "$$$$$ARG SIZE NOT EQUAL" << subTypes.size() << " " << ano->subTypes.size() << std::endl;
            return false;
        }

        bool compatible = true;
        for (int i = 0; i < subTypes.size(); i++)
        {
            std::cout << "TEST INIT " << subTypes[i]->GetTypeId() << " " << ano->subTypes[i]->GetTypeId() << std::endl;
            compatible &= subTypes[i]->InitCompatible(std::move(ano->subTypes[i]));
        }
        return compatible;
    }

    bool TupleType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        if (anotherType->GetTypeId() != TUPLE)
            return false;
        auto ano(UniquePtrCast<TupleType>(anotherType));
        if (ano->subTypes.size() != subTypes.size())
            return false;
        bool compatible = true;
        for (int i = 0; i < subTypes.size(); i++)
            compatible &= subTypes[i]->AssignCompatible(std::move(ano->subTypes[i]));
        return compatible;
    }

    //FuncType

    std::unique_ptr<TypeInfo> FuncType::Copy()
    {
        TypeInfo *ret = new FuncType(UniquePtrCast<TupleType>(argTypes->Copy()), retType->Copy());
        return std::unique_ptr<TypeInfo>(ret);
    }
    bool FuncType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        return false;
    }
    bool FuncType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        //TEMP FALSE
        return false;
    }

    std::unique_ptr<TypeInfo> FuncType::CalcFuncType(std::unique_ptr<TupleType> &&argTypes, bool &ok)
    {
        if (!this->argTypes->InitCompatible(std::move(argTypes)))
        {
            std::cout << "$$$$$FUNCTYPE NOT COMPATIBLE" << std::endl;
            ok = false;
        }

        return retType->Copy();
    }

    //ArrayType

    std::unique_ptr<TypeInfo> ArrayType::Copy()
    {
        TypeInfo *ret = new ArrayType(dimensions, contentType->Copy());
        return std::unique_ptr<TypeInfo>(ret);
    }

    bool ArrayType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        return false;
    }

    bool ArrayType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        //TEMP FALSE
        return false;
    }

    std::unique_ptr<TypeInfo> ArrayType::CalcArrayType(std::unique_ptr<TupleType> &&idTypes, bool &ok)
    {
        auto subTypes(idTypes->GetSubTypes());
        if (subTypes.size() != dimensions.size())
        {
            std::cout << "$$$$$ARRAY DIMENSION ERROR" << std::endl;
            ok = false;
        }

        for (auto &type : subTypes)
            ok &= type->IsBasicType() && (type->GetTypeId() != REAL);
        return contentType->Copy();
    }

    //WrapperType
    std::unique_ptr<TypeInfo> WrapperType::CalcType(std::unique_ptr<TypeInfo> &&anotherType, bool &ok)
    {
        if (anotherType->IsWrapperType())
            anotherType = UniquePtrCast<WrapperType>(anotherType)->DeWrap();
        TypeInfo *ret = new RValueType(targetType->CalcType(std::move(anotherType), ok));
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> WrapperType::CalcFuncType(std::unique_ptr<TupleType> &&argTypes, bool &ok)
    {
        TypeInfo *ret = new RValueType(targetType->CalcFuncType(std::move(argTypes), ok));
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> WrapperType::CalcArrayType(std::unique_ptr<TupleType> &&idTypes, bool &ok)
    {
        TypeInfo *ret = new RValueType(targetType->CalcArrayType(std::move(idTypes), ok));
        return std::unique_ptr<TypeInfo>(ret);
    }

    //LValueType
    std::unique_ptr<TypeInfo> LValueType::Copy()
    {
        TypeInfo *lValueType = new LValueType(targetType->Copy());
        return std::unique_ptr<TypeInfo>(lValueType);
    }

    bool LValueType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        return AssignCompatible(std::move(anotherType));
    }

    bool LValueType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        if (anotherType->IsWrapperType())
        {
            std::cout << "ASSIGN COMPATIBLE TEST " << targetType->GetTypeId() << std::endl;
            auto ano(UniquePtrCast<WrapperType>(anotherType)->DeWrap());
            return targetType->AssignCompatible(std::move(ano));
        }
        return targetType->AssignCompatible(std::move(anotherType));
    }

    //RValueType
    std::unique_ptr<TypeInfo> RValueType::Copy()
    {
        TypeInfo *rValueType = new RValueType(targetType->Copy());
        return std::unique_ptr<TypeInfo>(rValueType);
    }
    bool RValueType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        return false;
    }
    bool RValueType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        return false;
    }

    //RefType
    std::unique_ptr<TypeInfo> RefType::Copy()
    {
        TypeInfo *refValueType = new RefType(targetType->Copy());
        return std::unique_ptr<TypeInfo>(refValueType);
    }

    bool RefType::InitCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        TypeID tid = anotherType->GetTypeId();
        std::cout << "REF COMP " << tid << std::endl;
        if (tid == REF || tid == LVALUE)
        {
            auto ano(UniquePtrCast<WrapperType>(anotherType)->DeWrap());
            std::cout << ano->GetTypeId() << std::endl;
            return targetType->InitCompatible(std::move(ano));
        }
        return false;
    }

    bool RefType::AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        if (anotherType->IsWrapperType())
        {
            auto ano(UniquePtrCast<WrapperType>(anotherType)->DeWrap());
            return targetType->AssignCompatible(std::move(ano));
        }
        return targetType->AssignCompatible(std::move(anotherType));
    }
}