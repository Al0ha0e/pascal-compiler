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

    std::unique_ptr<TypeInfo> VOIDType::Copy()
    {
        TypeInfo *ret = new VOIDType();
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> IntegerType::Copy()
    {
        TypeInfo *ret = new IntegerType();
        return std::unique_ptr<TypeInfo>(ret);
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

    std::unique_ptr<TypeInfo> RealType::Copy()
    {
        TypeInfo *ret = new RealType();
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> RealType::CalcType(std::unique_ptr<TypeInfo> &&anotherType)
    {
        if (anotherType->IsBasicType())
        {
            return GenType(REAL);
        }
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> CharType::Copy()
    {
        TypeInfo *ret = new CharType();
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> CharType::CalcType(std::unique_ptr<TypeInfo> &&anotherType)
    {
        if (anotherType->IsBasicType())
        {
            return std::move(anotherType);
        }
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> BooleanType::Copy()
    {
        TypeInfo *ret = new BooleanType();
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> BooleanType::CalcType(std::unique_ptr<TypeInfo> &&anotherType)
    {
        if (anotherType->IsBasicType())
        {
            return std::move(anotherType);
        }
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> TupleType::Copy()
    {
        TypeInfo *ret = new TupleType(subTypes);
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> FuncType::Copy()
    {
        TypeInfo *ret = new FuncType(UniquePtrCast<TupleType>(argTypes->Copy()), isRef, retType->Copy());
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> FuncType::CalcFuncType(std::unique_ptr<TupleType> &&argTypes)
    {
        //TODO:
        return retType->Copy();
    }

    std::unique_ptr<TypeInfo> ArrayType::Copy()
    {
        TypeInfo *ret = new ArrayType(dimensions, contentType->Copy());
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> ArrayType::CalcArrayType(std::unique_ptr<TupleType> &&idTypes)
    {
        //TODO:
        return contentType->Copy();
    }
}