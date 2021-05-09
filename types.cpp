#include "types.h"

namespace PascalAST
{

    std::unique_ptr<TypeInfo> TypeInfo::CalcType(std::unique_ptr<TypeInfo> &&anotherType)
    {
        //TODO: Error Handling
        if (anotherType->IsBasicType())
        {
            return std::move(anotherType);
        }
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> TypeInfo::CalcFuncType(std::unique_ptr<TupleType> &&argTypes)
    {
        //TODO: Error Handling
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> TypeInfo::CalcArrayType(std::unique_ptr<TupleType> &&idTypes)
    {
        //TODO: Error Handling
        return GenType(VOID);
    }

    //VOIDType
    std::unique_ptr<TypeInfo> VOIDType::Copy()
    {
        TypeInfo *ret = new VOIDType();
        return std::unique_ptr<TypeInfo>(ret);
    }

    bool VOIDType::Compatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        return false;
    }

    //IntegerType

    std::unique_ptr<TypeInfo> IntegerType::Copy()
    {
        TypeInfo *ret = new IntegerType();
        return std::unique_ptr<TypeInfo>(ret);
    }

    bool IntegerType::Compatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        if (anotherType->IsBasicType())
            return true;
        return false;
    }

    std::unique_ptr<TypeInfo> IntegerType::CalcType(std::unique_ptr<TypeInfo> &&anotherType)
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

    bool RealType::Compatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        if (anotherType->IsBasicType())
            return true;
        return false;
    }

    std::unique_ptr<TypeInfo> RealType::CalcType(std::unique_ptr<TypeInfo> &&anotherType)
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

    bool CharType::Compatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        if (anotherType->IsBasicType())
            return true;
        return false;
    }

    std::unique_ptr<TypeInfo> CharType::CalcType(std::unique_ptr<TypeInfo> &&anotherType)
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

    bool BooleanType::Compatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        if (anotherType->IsBasicType())
            return true;
        return false;
    }

    std::unique_ptr<TypeInfo> BooleanType::CalcType(std::unique_ptr<TypeInfo> &&anotherType)
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

    bool TupleType::Compatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        if (anotherType->GetTypeId() != TUPLE)
            return false;
        auto ano(UniquePtrCast<TupleType>(anotherType));
        if (ano->subTypes.size() != subTypes.size())
            return false;
        bool compatible = true;
        for (int i = 0; i < subTypes.size(); i++)
            compatible &= subTypes[i]->Compatible(std::move(ano->subTypes[i]));
        return compatible;
    }

    //FuncType

    std::unique_ptr<TypeInfo> FuncType::Copy()
    {
        TypeInfo *ret = new FuncType(UniquePtrCast<TupleType>(argTypes->Copy()), retType->Copy());
        return std::unique_ptr<TypeInfo>(ret);
    }

    bool FuncType::Compatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        //TEMP FALSE
        return false;
    }

    std::unique_ptr<TypeInfo> FuncType::CalcFuncType(std::unique_ptr<TupleType> &&argTypes)
    {
        //TODO:
        return retType->Copy();
    }

    //ArrayType

    std::unique_ptr<TypeInfo> ArrayType::Copy()
    {
        TypeInfo *ret = new ArrayType(dimensions, contentType->Copy());
        return std::unique_ptr<TypeInfo>(ret);
    }

    bool ArrayType::Compatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        //TEMP FALSE
        return false;
    }

    std::unique_ptr<TypeInfo> ArrayType::CalcArrayType(std::unique_ptr<TupleType> &&idTypes)
    {
        //TODO:
        return contentType->Copy();
    }

    //WrapperType
    std::unique_ptr<TypeInfo> WrapperType::CalcType(std::unique_ptr<TypeInfo> &&anotherType)
    {
        if (anotherType->IsWrapperType())
            anotherType = UniquePtrCast<WrapperType>(anotherType)->DeWrap();
        TypeInfo *ret = new RValueType(targetType->CalcType(std::move(anotherType)));
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> WrapperType::CalcFuncType(std::unique_ptr<TupleType> &&argTypes)
    {
        TypeInfo *ret = new RValueType(targetType->CalcFuncType(std::move(argTypes)));
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> WrapperType::CalcArrayType(std::unique_ptr<TupleType> &&idTypes)
    {
        TypeInfo *ret = new RValueType(targetType->CalcArrayType(std::move(idTypes)));
        return std::unique_ptr<TypeInfo>(ret);
    }

    //LValueType
    std::unique_ptr<TypeInfo> LValueType::Copy()
    {
        TypeInfo *lValueType = new LValueType(targetType->Copy());
        return std::unique_ptr<TypeInfo>(lValueType);
    }

    bool LValueType::Compatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        if (anotherType->IsWrapperType())
        {
            auto ano(UniquePtrCast<WrapperType>(anotherType)->DeWrap());
            return targetType->Compatible(std::move(ano));
        }
        return targetType->Compatible(std::move(anotherType));
    }

    //RValueType
    std::unique_ptr<TypeInfo> RValueType::Copy()
    {
        TypeInfo *rValueType = new RValueType(targetType->Copy());
        return std::unique_ptr<TypeInfo>(rValueType);
    }

    bool RValueType::Compatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        return false;
    }

    //RefType
    std::unique_ptr<TypeInfo> RefType::Copy()
    {
        TypeInfo *refValueType = new RefType(targetType->Copy());
        return std::unique_ptr<TypeInfo>(refValueType);
    }

    bool RefType::Compatible(std::unique_ptr<TypeInfo> &&anotherType)
    {
        TypeID tid = anotherType->GetTypeId();
        if (tid == REF || tid == LVALUE)
        {
            auto ano(UniquePtrCast<WrapperType>(anotherType)->DeWrap());
            return targetType->Compatible(std::move(ano));
        }
        return false;
    }
}