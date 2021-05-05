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
    std::unique_ptr<TypeInfo> TypeInfo::CalcFuncType(std::vector<std::unique_ptr<TypeInfo>> &&argTypes)
    {
        //TODO: Error Handling
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> TypeInfo::CalcArrayType(std::vector<std::unique_ptr<TypeInfo>> &&idTypes)
    {
        //TODO: Error Handling
        return GenType(VOID);
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

    std::unique_ptr<TypeInfo> RealType::CalcType(std::unique_ptr<TypeInfo> &&anotherType)
    {
        if (anotherType->IsBasicType())
        {
            return GenType(REAL);
        }
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> CharType::CalcType(std::unique_ptr<TypeInfo> &&anotherType)
    {
        if (anotherType->IsBasicType())
        {
            return std::move(anotherType);
        }
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> BooleanType::CalcType(std::unique_ptr<TypeInfo> &&anotherType)
    {
        if (anotherType->IsBasicType())
        {
            return std::move(anotherType);
        }
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> FuncType::CalcFuncType(std::vector<std::unique_ptr<TypeInfo>> &&argTypes)
    {
        //TODO:
        return GenType(retType);
    }

    std::unique_ptr<TypeInfo> ArrayType::CalcArrayType(std::vector<std::unique_ptr<TypeInfo>> &&idTypes)
    {
        //TODO:
        return GenType(contentType);
    }
}