#include <string>
#include <memory>
#include <vector>
#include <map>

namespace PascalAST
{

    template <typename TB, typename TA>
    inline std::unique_ptr<TB> UniquePtrCast(std::unique_ptr<TA> &&ori)
    {
        return std::unique_ptr<TB>(dynamic_cast<TB *>(ori->release()));
    }

    // template <typename T, typename... Ts>
    // std::unique_ptr<T> make_unique(Ts &&...params)
    // {
    //     return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
    // }

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

    class TupleType;

    class TypeInfo
    {
    public:
        // virtual std::string ToString() = 0;
        virtual std::unique_ptr<TypeInfo> CalcType(std::unique_ptr<TypeInfo> &&anotherType);
        virtual std::unique_ptr<TypeInfo> CalcFuncType(std::unique_ptr<TupleType> &&argTypes);
        virtual std::unique_ptr<TypeInfo> CalcArrayType(std::unique_ptr<TupleType> &&idTypes);
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
        TupleType(std::vector<std::unique_ptr<TypeInfo>> &&types) : TypeInfo(TUPLE)
        {
            for (int i = 0; i < types.size(); i++)
            {
                subTypes.push_back(std::move(types[i]));
            }
        }
        TupleType &operator+=(const TupleType &&ano)
        {
            for (int i = 0; i < ano.subTypes.size(); i++)
                subTypes.push_back(std::move(ano.subTypes[i]));
        }

        std::unique_ptr<TypeInfo> Copy();

    private:
        std::vector<std::unique_ptr<TypeInfo>> subTypes;
    };

    class FuncType : public TypeInfo
    {
    public:
        virtual std::unique_ptr<TypeInfo> CalcFuncType(std::unique_ptr<TupleType> &&argTypes);
        std::unique_ptr<TypeInfo> Copy();
        FuncType() : TypeInfo(FUNC) {}

        FuncType(std::unique_ptr<TupleType> &&argTypes, std::vector<bool> &isRef, std::unique_ptr<TypeInfo> &&retType)
            : argTypes(std::move(argTypes)), isRef(isRef), retType(std::move(retType)), TypeInfo(FUNC) {}

        FuncType(std::unique_ptr<TupleType> &&argTypes, std::vector<bool> &&isRef, std::unique_ptr<TypeInfo> &&retType)
            : argTypes(std::move(argTypes)), isRef(isRef), retType(std::move(retType)), TypeInfo(FUNC) {}

    private:
        std::unique_ptr<TupleType> argTypes;
        std::vector<bool> isRef;
        std::unique_ptr<TypeInfo> retType;
    };

    class ArrayType : public TypeInfo
    {
    public:
        virtual std::unique_ptr<TypeInfo> CalcArrayType(std::unique_ptr<TupleType> &&idTypes);
        std::unique_ptr<TypeInfo> Copy();
        ArrayType() : TypeInfo(ARRAY) {}
        ArrayType(int dimension, std::unique_ptr<TypeInfo> &&contentType) : dimension(dimension), contentType(std::move(contentType)), TypeInfo(ARRAY) {}

    private:
        int dimension;
        std::unique_ptr<TypeInfo> contentType;
    };

    inline std::unique_ptr<TypeInfo> GenType(TypeID id)
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

    inline std::unique_ptr<TypeInfo> GenTypeByStr(std::string id)
    {
        if (id == "int")
        {
            return GenType(INTEGER);
        }
        if (id == "real")
        {
            return GenType(REAL);
        }
        if (id == "char")
        {
            return GenType(CHAR);
        }
        if (id == "boolean")
        {
            return GenType(BOOLEAN);
        }
    }
}
