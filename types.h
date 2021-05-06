#include <string>
#include <memory>
#include <vector>
#include <map>

namespace PascalAST
{

    enum TypeID
    {
        VOID,
        BOOLEAN,
        INTEGER,
        REAL,
        CHAR,
        FUNC,
        ARRAY,
        TUPLE
    };

    class TypeInfo
    {
    public:
        // virtual std::string ToString() = 0;
        virtual std::unique_ptr<TypeInfo> CalcType(std::unique_ptr<TypeInfo> &&anotherType);
        virtual std::unique_ptr<TypeInfo> CalcFuncType(std::vector<std::unique_ptr<TypeInfo>> &&argTypes);
        virtual std::unique_ptr<TypeInfo> CalcArrayType(std::vector<std::unique_ptr<TypeInfo>> &&idTypes);
        virtual std::unique_ptr<TypeInfo> Copy() = 0;

        TypeInfo() {}
        TypeInfo(TypeID id) : id(id) {}

        bool Compatible(std::unique_ptr<TypeInfo> &&anotherType)
        {
        }
        TypeID GetTypeId()
        {
            return id;
        }
        bool IsBasicType()
        {
            if (id == BOOLEAN || id == REAL || id == CHAR || id == INTEGER)
                return true;
        }
        bool IsVoidType()
        {
            return id == VOID;
        }

    private:
        TypeID id;
    };

    class VOIDType : public TypeInfo
    {
        std::unique_ptr<TypeInfo> Copy();
    };

    class BooleanType : public TypeInfo
    {
        virtual std::unique_ptr<TypeInfo> CalcType(std::unique_ptr<TypeInfo> &&anotherType);
        std::unique_ptr<TypeInfo> Copy();
        BooleanType() : TypeInfo(BOOLEAN) {}
    };

    class IntegerType : public TypeInfo
    {
    public:
        virtual std::unique_ptr<TypeInfo> CalcType(std::unique_ptr<TypeInfo> &&anotherType);
        std::unique_ptr<TypeInfo> Copy();
        IntegerType() : TypeInfo(INTEGER) {}
    };

    class RealType : public TypeInfo
    {
    public:
        virtual std::unique_ptr<TypeInfo> CalcType(std::unique_ptr<TypeInfo> &&anotherType);
        std::unique_ptr<TypeInfo> Copy();
        RealType() : TypeInfo(REAL) {}
    };

    class CharType : public TypeInfo
    {
    public:
        virtual std::unique_ptr<TypeInfo> CalcType(std::unique_ptr<TypeInfo> &&anotherType);
        std::unique_ptr<TypeInfo> Copy();
        CharType() : TypeInfo(CHAR) {}
    };

    class TupleType : public TypeInfo
    {
    public:
        TupleType() : TypeInfo(TUPLE) {}
        TupleType(std::vector<std::unique_ptr<TypeInfo>> &types) : TypeInfo(TUPLE)
        {
            for (int i = 0; i < types.size(); i++)
            {
                subTypes.push_back(types[i]->Copy());
            }
        }

        std::unique_ptr<TypeInfo> Copy();

    private:
        std::vector<std::unique_ptr<TypeInfo>> subTypes;
    };

    class FuncType : public TypeInfo
    {
    public:
        virtual std::unique_ptr<TypeInfo> CalcFuncType(std::vector<std::unique_ptr<TypeInfo>> &&argTypes);
        std::unique_ptr<TypeInfo> Copy();
        FuncType() : TypeInfo(FUNC) {}

        FuncType(std::vector<TypeID> &argTypes, std::vector<bool> &isRef, TypeID retType) : argTypes(argTypes), isRef(isRef), retType(retType), TypeInfo(FUNC) {}

    private:
        std::vector<TypeID> argTypes;
        std::vector<bool> isRef;
        TypeID retType;
    };

    class ArrayType : public TypeInfo
    {
    public:
        virtual std::unique_ptr<TypeInfo> CalcArrayType(std::vector<std::unique_ptr<TypeInfo>> &&idTypes);
        std::unique_ptr<TypeInfo> Copy();
        ArrayType() : TypeInfo(ARRAY) {}
        ArrayType(int dimension, TypeID contentType) : dimension(dimension), contentType(contentType), TypeInfo(ARRAY) {}

    private:
        int dimension;
        TypeID contentType;
    };

    std::unique_ptr<TypeInfo> GenType(TypeID id)
    {
        TypeInfo *ret;
        if (id == VOID)
        {
            ret = new VOIDType();
        }
        else if (id == INTEGER)
        {
            ret = new IntegerType();
        }
        else if (id == REAL)
        {
            ret = new RealType();
        }
        else if (id == CHAR)
        {
            ret = new CharType();
        }
        return std::unique_ptr<TypeInfo>(ret);
    }

}
