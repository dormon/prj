#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<functional>
#include<stdexcept>
#include<cassert>
#include<sstream>

class Type{
  public:
    enum KindOfType{
      QUALIFIED  ,
      UNQUALIFIED,
      EMPTY      ,
    };
    KindOfType kindOfType;
    Type(KindOfType const&t):kindOfType(t){}
    virtual ~Type(){}
};

class EmptyType: public Type{
  public:
    EmptyType():Type(EMPTY){}
};

class UnqualifiedType: public Type{
  public:
    enum KindOfUnqualifiedType{
      BIT      ,
      ANY      ,
      COMPOSITE,
    };
    KindOfUnqualifiedType kindOfUnqualifiedType;
    UnqualifiedType(KindOfUnqualifiedType const&t):Type(UNQUALIFIED),kindOfUnqualifiedType(t){}
};

class QualifiedType: public Type{
  public:
    enum KindOfQualifier{
      IN    = 1<<0    ,
      OUT   = 1<<1    ,
      INOUT = IN | OUT,
    };
    KindOfQualifier                 qualifier;
    std::shared_ptr<UnqualifiedType>type     ;
    QualifiedType(KindOfQualifier const&q,std::shared_ptr<UnqualifiedType>const&t):Type(QUALIFIED),qualifier(q),type(t){}
};

class BitType: public UnqualifiedType{
  public:
    BitType():UnqualifiedType(BIT){}
};

class CompositeType: public UnqualifiedType{
  public:
    enum KindOfCompositeType{
      STRUCT  ,
      VECTOR  ,
      FUNCTION,
    };
    KindOfCompositeType kindOfCompositeType;
    CompositeType(KindOfCompositeType const&t):UnqualifiedType(COMPOSITE),kindOfCompositeType(t){}
};

class AnyType: public UnqualifiedType{
  public:
    AnyType():UnqualifiedType(ANY){}
};

class StructType: public CompositeType{
  public:
    std::vector<std::shared_ptr<UnqualifiedType>>componentTypes;
    StructType(std::vector<std::shared_ptr<UnqualifiedType>>const&t):CompositeType(STRUCT),componentTypes(t){}
};

class VectorType: public CompositeType{
  public:
    enum KindOfVector{
      INFINITE,
      ARRAY   ,
    };
    KindOfVector kindOfVector;
    std::shared_ptr<UnqualifiedType>type;
    VectorType(std::shared_ptr<UnqualifiedType>const&t):CompositeType(VECTOR),kindOfVector(INFINITE),type(t){}
    VectorType(KindOfVector const&v,std::shared_ptr<UnqualifiedType>const&t):CompositeType(VECTOR),kindOfVector(v),type(t){}
};

class ArrayType: public VectorType{
  public:
    size_t size;
    ArrayType(std::shared_ptr<UnqualifiedType>const&t,size_t const&s):VectorType(ARRAY,t),size(s){}
};

class FunctionType: public CompositeType{
  public:
    std::vector<std::shared_ptr<QualifiedType>>parameterTypes;
    FunctionType(std::vector<std::shared_ptr<QualifiedType>>const&t):CompositeType(FUNCTION),parameterTypes(t){}
};



class Value{
  public:
    enum KindOfValue{
      CONSTANT,
      VARIABlE,
    };
    KindOfValue kindOfValue;
    std::shared_ptr<UnqualifiedType>type;
    Value(KindOfValue const&k,std::shared_ptr<UnqualifiedType>const&t):kindOfValue(k),type(t){}
};

class Constant: public Value{
  public:
    enum KindOfConstant{
      BUILTIN ,
      EXTERNAL,
    };
    Constant(std::shared_ptr<UnqualifiedType>const&t):Value(CONSTANT,t){}
};

class BuiltInConstant: public Constant{
  public:
    enum KindOfBuiltInConstant{
      BIT_CONSTANT     ,
      TYPENAME_CONSTANT,
    };
    KindOfBuiltInConstant kindOfBuiltInConstant;
    BuiltInConstant(KindOfBuiltInConstant const&k,std::shared_ptr<UnqualifiedType>const&t):Constant(t),kindOfBuiltInConstant(k){}
};

class BitConstant: public BuiltInConstant{
  public:
    enum KindOfBitConstant{
      ZERO,
      ONE ,
    };
    KindOfBitConstant kindOfBitConstant;
    BitConstant(KindOfBitConstant const&k):BuiltInConstant(BIT_CONSTANT,std::make_shared<BitType>()),kindOfBitConstant(k){}
};

class BitZeroConstant: public BitConstant{
  public:
    BitZeroConstant():BitConstant(ZERO){}
};

class BitOneConstant: public BitConstant{
  public:
    BitOneConstant():BitConstant(ONE){}
};


class Resource{
  public:
    std::shared_ptr<UnqualifiedType>type;
  protected:
    void*data;
};

class Statement{
  public:
    enum KindOfStatement{
      FUNCTION,
      BODY    ,
      IF      ,
      WHILE   ,
    };
    KindOfStatement kindOfStatement;
    std::shared_ptr<FunctionType>functionType;
    Statement(KindOfStatement const&t):kindOfStatement(t){}
};

class Function: public Statement{
  public:
    enum KindOfFunction{
      ATOMIC   ,
      COMPOSITE,
    };
    KindOfFunction kindOfFunction;
    Function(KindOfFunction const&k,std::shared_ptr<FunctionType>const&t):Statement(FUNCTION),kindOfFunction(k){
      functionType = t;
    }
};

class Body: public Statement{
  public:
    std::vector<std::shared_ptr<Statement>>statements;
    Body(std::vector<std::shared_ptr<Statement>>const&s):Statement(BODY),statements(s){}
};

class If: public Statement{
  public:
    std::shared_ptr<Resource >conditionVariable;
    std::shared_ptr<Statement>statement        ;
    If(std::shared_ptr<Resource>const&var,std::shared_ptr<Statement>const&s):Statement(IF),conditionVariable(var),statement(s){}
};

class While: public Statement{
  public:
    std::shared_ptr<Resource >conditionVariable;
    std::shared_ptr<Statement>statement        ;
    While(std::shared_ptr<Resource>const&var,std::shared_ptr<Statement>const&s):Statement(WHILE),conditionVariable(var),statement(s){}
};

class AtomicFunction: public Function{
  public:
    enum AtomicFunctionType{
      BUILTIN ,
      EXTERNAL,
    };
    AtomicFunctionType atomicFunctionType;
    AtomicFunction(AtomicFunctionType const&a,std::shared_ptr<FunctionType>const&t):Function(ATOMIC,t),atomicFunctionType(a){}
};

template<typename T,typename...ARGS>
inline std::shared_ptr<T>shared(ARGS const&...args){
  return std::make_shared<T>(args...);
}

class BuiltInFunction: public AtomicFunction{
  public:
    enum BuiltInFunctionType{
      NAND        ,
      IS_TYPE     ,
      IS_QUALIFIED,
      IS_IN       ,
      IS_OUT      ,
      IS_BIT      ,
      IS_ANY      ,
      IS_STRUCT   ,
      IS_FUNCTION ,
      IS_VECTOR   ,
      TYPE_COMPARE,
    };
    BuiltInFunctionType builtInFunctionType;
    BuiltInFunction(BuiltInFunctionType const&b,std::shared_ptr<FunctionType>const&t):AtomicFunction(BUILTIN,t),builtInFunctionType(b){}
};

class NandFunction: public BuiltInFunction{
  public:
    NandFunction():BuiltInFunction(NAND,
        shared<FunctionType>(std::vector<std::shared_ptr<QualifiedType>>{
          std::make_shared<QualifiedType>(QualifiedType::OUT,std::make_shared<BitType>()),
          std::make_shared<QualifiedType>(QualifiedType::IN ,std::make_shared<BitType>()),
          std::make_shared<QualifiedType>(QualifiedType::IN ,std::make_shared<BitType>()),
          })){}
};

class TypeCompareFunction: public BuiltInFunction{
  public:
    TypeCompareFunction():BuiltInFunction(TYPE_COMPARE,
        shared<FunctionType>(std::vector<std::shared_ptr<QualifiedType>>{
          std::make_shared<QualifiedType>(QualifiedType::OUT,std::make_shared<BitType>()),
          std::make_shared<QualifiedType>(QualifiedType::IN ,std::make_shared<AnyType>()),
          std::make_shared<QualifiedType>(QualifiedType::IN ,std::make_shared<AnyType>()),
          })){}
};

std::string typeToString(std::shared_ptr<StructType     >const&);
std::string typeToString(std::shared_ptr<BitType        >const&);
std::string typeToString(std::shared_ptr<AnyType        >const&);
std::string typeToString(std::shared_ptr<CompositeType  >const&);
std::string typeToString(std::shared_ptr<UnqualifiedType>const&);
std::string typeToString(std::shared_ptr<QualifiedType  >const&);
std::string typeToString(std::shared_ptr<Type           >const&);

std::string typeToString(std::shared_ptr<FunctionType>const&t){
  std::stringstream ss;
  ss << "(";
  for(auto const&x:t->parameterTypes)
    ss << typeToString(x) << ";";
  ss << ")";
  return ss.str();
}

std::string typeToString(std::shared_ptr<StructType>const&t){
  std::stringstream ss;
  ss << "{";
  for(auto const&x:t->componentTypes)
    ss << typeToString(x) << ";";
  ss << "}";
  return ss.str();
}

std::string typeToString(std::shared_ptr<VectorType>const&t){
  std::stringstream ss;
  ss << typeToString(t->type);
  ss << "[";
  if(t->kindOfVector == VectorType::ARRAY)
    ss << std::dynamic_pointer_cast<ArrayType>(t)->size;
  ss << "]";
  return ss.str();
}

std::string typeToString(std::shared_ptr<BitType>const&){
  return "bit";
}

std::string typeToString(std::shared_ptr<AnyType>const&){
  return "any";
}

std::string typeToString(std::shared_ptr<CompositeType>const&t){
  switch(t->kindOfCompositeType){
    case CompositeType::STRUCT  :return typeToString(std::dynamic_pointer_cast<StructType  >(t));
    case CompositeType::VECTOR  :return typeToString(std::dynamic_pointer_cast<VectorType  >(t));
    case CompositeType::FUNCTION:return typeToString(std::dynamic_pointer_cast<FunctionType>(t));
  }
  return "";
}

std::string typeToString(std::shared_ptr<UnqualifiedType>const&t){
  switch(t->kindOfUnqualifiedType){
    case UnqualifiedType::COMPOSITE:return typeToString(std::dynamic_pointer_cast<CompositeType>(t));
    case UnqualifiedType::BIT      :return typeToString(std::dynamic_pointer_cast<BitType      >(t));
    case UnqualifiedType::ANY      :return typeToString(std::dynamic_pointer_cast<AnyType      >(t));
  }
  return "";
}

std::string typeToString(std::shared_ptr<QualifiedType>const&t){
  std::stringstream ss;
  switch(t->qualifier){
    case QualifiedType::IN   :ss << "in"   ;break;
    case QualifiedType::OUT  :ss << "out"  ;break;
    case QualifiedType::INOUT:ss << "inout";break;
  }
  ss << " ";
  ss << typeToString(t->type);
  return ss.str();
}

std::string typeToString(std::shared_ptr<EmptyType>const&){
  return "empty";
}

std::string typeToString(std::shared_ptr<Type>const&t){
  switch(t->kindOfType){
    case Type::QUALIFIED  :return typeToString(std::dynamic_pointer_cast<QualifiedType  >(t));
    case Type::UNQUALIFIED:return typeToString(std::dynamic_pointer_cast<UnqualifiedType>(t));
    case Type::EMPTY      :return typeToString(std::dynamic_pointer_cast<EmptyType      >(t));
  }
  return "";
};

int main(){
  std::cout << typeToString(shared<NandFunction>()->functionType) << std::endl;
  return 0;
}
