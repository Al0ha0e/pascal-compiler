#ifndef TYPES
#define TYPES

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <iostream>

namespace PascalAST
{

    template <typename TB, typename TA>
    inline std::unique_ptr<TB> UniquePtrCast(std::unique_ptr<TA> &&ori)
    {
        return std::unique_ptr<TB>(dynamic_cast<TB *>(ori.release()));
    }

    template <typename TB, typename TA>
    inline std::unique_ptr<TB> UniquePtrCast(std::unique_ptr<TA> &ori)
    {
        return std::unique_ptr<TB>(dynamic_cast<TB *>(ori.release()));
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
        TUPLE,
        LVALUE,
        RVALUE,
        REF
    };

    class TupleType;

    class TypeInfo
    {
    public:
        virtual std::unique_ptr<TypeInfo> CalcType(std::unique_ptr<TypeInfo> &&anotherType, std::string op, bool &ok, std::string &errMsg);
        virtual std::unique_ptr<TypeInfo> CalcFuncType(std::unique_ptr<TupleType> &&argTypes, bool &ok, std::string &errMsg);
        virtual std::unique_ptr<TypeInfo> CalcArrayType(std::unique_ptr<TupleType> &&idTypes, bool &ok, std::string &errMsg);
        virtual std::unique_ptr<TypeInfo> Copy() = 0;
        virtual bool InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg) = 0;
        virtual bool AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg) = 0;
        virtual std::string ToString() = 0;

        TypeInfo() {}
        TypeInfo(TypeID id) : id(id) {}

        TypeID GetTypeId()
        {
            return id;
        }
        bool IsBasicType()
        {
            if (id == BOOLEAN || id == REAL || id == CHAR || id == INTEGER)
                return true;
            return false;
        }
        bool IsVoidType()
        {
            return id == VOID;
        }
        bool IsWrapperType()
        {
            if (id == LVALUE || id == RVALUE || id == REF)
                return true;
            return false;
        }

    private:
        TypeID id;
    };

    class WrapperType : public TypeInfo
    {
    public:
        WrapperType() : TypeInfo(VOID) {}
        WrapperType(TypeID id) : TypeInfo(id) {}
        WrapperType(std::unique_ptr<TypeInfo> &&target, TypeID id) : targetType(std::move(target)), TypeInfo(id)
        {
            if (targetType->IsWrapperType())
                targetType = UniquePtrCast<WrapperType>(targetType)->DeWrap();
        }

        virtual std::unique_ptr<TypeInfo> CalcType(std::unique_ptr<TypeInfo> &&anotherType, std::string op, bool &ok, std::string &errMsg) override;
        virtual std::unique_ptr<TypeInfo> CalcFuncType(std::unique_ptr<TupleType> &&argTypes, bool &ok, std::string &errMsg) override;
        virtual std::unique_ptr<TypeInfo> CalcArrayType(std::unique_ptr<TupleType> &&idTypes, bool &ok, std::string &errMsg) override;

        std::unique_ptr<TypeInfo> DeWrap()
        {
            //std::cout << "DEWRAP " << targetType->GetTypeId() << std::endl;
            return targetType->Copy();
        }

    protected:
        std::unique_ptr<TypeInfo> targetType;
    };

    class LValueType : public WrapperType
    {
    public:
        LValueType() : WrapperType(LVALUE) {}
        LValueType(std::unique_ptr<TypeInfo> &&targetType) : WrapperType(std::move(targetType), LVALUE)
        {
        }

        std::unique_ptr<TypeInfo> Copy();
        std::string ToString();
        bool InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);
        bool AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);
    };

    class RValueType : public WrapperType
    {
    public:
        RValueType() : WrapperType(RVALUE) {}
        RValueType(std::unique_ptr<TypeInfo> &&targetType) : WrapperType(std::move(targetType), RVALUE) {}

        std::unique_ptr<TypeInfo> Copy();
        std::string ToString();
        bool InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);
        bool AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);
    };

    class RefType : public WrapperType
    {
    public:
        RefType() : WrapperType(REF) {}
        RefType(std::unique_ptr<TypeInfo> &&targetType) : WrapperType(std::move(targetType), REF) {}

        std::unique_ptr<TypeInfo> Copy();
        std::string ToString();
        bool InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);
        bool AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);
    };

    class VOIDType : public TypeInfo
    {
        std::unique_ptr<TypeInfo> Copy();
        std::string ToString();
        bool InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);
        bool AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);
    };

    class BooleanType : public TypeInfo
    {
        virtual std::unique_ptr<TypeInfo> CalcType(std::unique_ptr<TypeInfo> &&anotherType, std::string op, bool &ok, std::string &errMsg) override;
        std::unique_ptr<TypeInfo> Copy();
        std::string ToString();
        BooleanType() : TypeInfo(BOOLEAN) {}
        bool InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);
        bool AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);
    };

    class IntegerType : public TypeInfo
    {
    public:
        virtual std::unique_ptr<TypeInfo> CalcType(std::unique_ptr<TypeInfo> &&anotherType, std::string op, bool &ok, std::string &errMsg) override;
        std::unique_ptr<TypeInfo> Copy();
        std::string ToString();
        IntegerType() : TypeInfo(INTEGER) {}
        bool InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);
        bool AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);
    };

    class RealType : public TypeInfo
    {
    public:
        virtual std::unique_ptr<TypeInfo> CalcType(std::unique_ptr<TypeInfo> &&anotherType, std::string op, bool &ok, std::string &errMsg) override;
        std::unique_ptr<TypeInfo> Copy();
        std::string ToString();
        RealType() : TypeInfo(REAL) {}
        bool InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);
        bool AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);
    };

    class CharType : public TypeInfo
    {
    public:
        virtual std::unique_ptr<TypeInfo> CalcType(std::unique_ptr<TypeInfo> &&anotherType, std::string op, bool &ok, std::string &errMsg) override;
        std::unique_ptr<TypeInfo> Copy();
        std::string ToString();
        CharType() : TypeInfo(CHAR) {}
        bool InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);
        bool AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);
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

        std::unique_ptr<TypeInfo> Copy();
        std::string ToString();
        bool InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);
        bool AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);

        std::vector<std::unique_ptr<TypeInfo>> GetSubTypes()
        {
            std::vector<std::unique_ptr<TypeInfo>> ret;
            for (auto &type : subTypes)
                ret.push_back(type->Copy());
            return std::move(ret);
        }

    private:
        std::vector<std::unique_ptr<TypeInfo>> subTypes;
    };

    class FuncType : public TypeInfo
    {
    public:
        virtual std::unique_ptr<TypeInfo> CalcFuncType(std::unique_ptr<TupleType> &&argTypes, bool &ok, std::string &errMsg) override;
        std::unique_ptr<TypeInfo> Copy();
        std::string ToString();
        FuncType() : TypeInfo(FUNC) {}

        FuncType(std::unique_ptr<TupleType> &&argTypes, std::unique_ptr<TypeInfo> &&retType)
            : argTypes(std::move(argTypes)), retType(std::move(retType)), TypeInfo(FUNC) {}

        bool InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);
        bool AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);

        std::vector<std::unique_ptr<TypeInfo>> GetArgTypes()
        {
            return argTypes->GetSubTypes();
        }

        std::unique_ptr<TypeInfo> RetAsLValue()
        {
            TypeInfo *lval = new LValueType(retType->Copy());
            return std::unique_ptr<TypeInfo>(lval);
        }

    private:
        std::unique_ptr<TupleType> argTypes;
        std::unique_ptr<TypeInfo> retType;
    };

    class ArrayType : public TypeInfo
    {
    public:
        virtual std::unique_ptr<TypeInfo> CalcArrayType(std::unique_ptr<TupleType> &&idTypes, bool &ok, std::string &errMsg) override;
        std::unique_ptr<TypeInfo> Copy();
        std::string ToString();
        ArrayType() : TypeInfo(ARRAY) {}
        ArrayType(std::vector<std::pair<int, int>> &dimensions, std::unique_ptr<TypeInfo> &&contentType)
            : dimensions(dimensions), contentType(std::move(contentType)), TypeInfo(ARRAY) {}

        bool InitCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);
        bool AssignCompatible(std::unique_ptr<TypeInfo> &&anotherType, std::string &errMsg);
        std::vector<int> GetOffset()
        {
            std::vector<int> ret;
            for (auto &dim : dimensions)
                ret.push_back(dim.first);
            return ret;
        }

    private:
        std::vector<std::pair<int, int>> dimensions;
        std::unique_ptr<TypeInfo> contentType;
    };

    inline std::unique_ptr<TypeInfo> GenType(TypeID id)
    {
        TypeInfo *ret = new VOIDType();
        if (id == INTEGER)
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
        //std::cout << "TYPESTR " << id << std::endl;
        if (id == "integer" || id == "int")
        {
            return GenType(INTEGER);
        }
        if (id == "real" || id == "float")
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
        return GenType(VOID);
    }
}

#endif