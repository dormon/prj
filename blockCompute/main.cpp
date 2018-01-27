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

enum class InOut{
  IN    = 1<<0    ,
  OUT   = 1<<1    ,
  INOUT = IN | OUT,
};

class Type{
  public:
    enum TypeOfType{
      QUALIFIED  ,
      UNQUALIFIED,
    };
    TypeOfType typeOfType;
    Type(TypeOfType const&t):typeOfType(t){}
    virtual ~Type(){}
};

class UnqualifiedType: public Type{
  public:
    enum TypeOfUnqualifiedType{
      BIT      ,
      COMPOSITE,
      ANY      ,
      TYPENAME ,
    };
    TypeOfUnqualifiedType typeOfUnqualifiedType;
    UnqualifiedType(TypeOfUnqualifiedType const&t):Type(UNQUALIFIED),typeOfUnqualifiedType(t){}
};

class QualifiedType: public Type{
  public:
    enum InOut{
      IN    = 1<<0    ,
      OUT   = 1<<1    ,
      INOUT = IN | OUT,
    };
    InOut                           inOut;
    std::shared_ptr<UnqualifiedType>type ;
    QualifiedType(InOut const&q,std::shared_ptr<UnqualifiedType>const&t):Type(QUALIFIED),inOut(q),type(t){}
};

class BitType: public UnqualifiedType{
  public:
    BitType():UnqualifiedType(BIT){}
};

class CompositeType: public UnqualifiedType{
  public:
    enum TypeOfCompositeType{
      STRUCT  ,
      VECTOR  ,
      FUNCTION,
    };
    TypeOfCompositeType typeOfCompositeType;
    CompositeType(TypeOfCompositeType const&t):UnqualifiedType(COMPOSITE),typeOfCompositeType(t){}
};

class AnyType: public UnqualifiedType{
  public:
    AnyType():UnqualifiedType(ANY){}
};

class TypenameType: public UnqualifiedType{
  public:
    TypenameType():UnqualifiedType(TYPENAME){}
};

class StructType: public CompositeType{
  public:
    std::vector<std::shared_ptr<UnqualifiedType>>componentTypes;
    StructType(std::vector<std::shared_ptr<UnqualifiedType>>const&t):CompositeType(STRUCT),componentTypes(t){}
};

class VectorType: public CompositeType{
  public:
    enum TypeOfVector{
      INFINITE,
      ARRAY   ,
    };
    TypeOfVector typeOfVector;
    std::shared_ptr<UnqualifiedType>type;
    VectorType(std::shared_ptr<UnqualifiedType>const&t):CompositeType(VECTOR),typeOfVector(INFINITE),type(t){}
    VectorType(TypeOfVector const&v,std::shared_ptr<UnqualifiedType>const&t):CompositeType(VECTOR),typeOfVector(v),type(t){}
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

class Resource{
  public:
    std::shared_ptr<UnqualifiedType>type;
  protected:
    void*data;
};

class Statement{
  public:
    enum StatementType{
      FUNCTION,
      BODY    ,
      IF      ,
      WHILE   ,
    };
    StatementType statementType;
    Statement(StatementType const&t):statementType(t){}
};

class Function: public Statement{
  public:
    enum FunctionKind{
      ATOMIC   ,
      COMPOSITE,
    };
    FunctionKind functionKind;
    std::shared_ptr<FunctionType>functionType;
    Function(FunctionKind const&k,std::shared_ptr<FunctionType>const&t):Statement(FUNCTION),functionKind(k),functionType(t){}
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

class BuiltInFunction: public AtomicFunction{
  public:
    enum BuiltInFunctionType{
      NAND        ,
      TYPE_COMPARE,
    };
    BuiltInFunctionType builtInFunctionType;
    BuiltInFunction(BuiltInFunctionType const&b,std::shared_ptr<FunctionType>const&t):AtomicFunction(BUILTIN,t),builtInFunctionType(b){}
};

//class NandFunction: public BuiltInFunction{
//  public:
//    NandFunction():BuiltInFunction(NAND,std::make_shared<FunctionType>({nullptr})){}
//};

std::string typeToString(std::shared_ptr<StructType     >const&);
std::string typeToString(std::shared_ptr<BitType        >const&);
std::string typeToString(std::shared_ptr<AnyType        >const&);
std::string typeToString(std::shared_ptr<TypenameType   >const&);
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
  if(t->typeOfVector == VectorType::ARRAY)
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

std::string typeToString(std::shared_ptr<TypenameType>const&){
  return "typename";
}

std::string typeToString(std::shared_ptr<CompositeType>const&t){
  switch(t->typeOfCompositeType){
    case CompositeType::STRUCT  :return typeToString(std::dynamic_pointer_cast<StructType  >(t));
    case CompositeType::VECTOR  :return typeToString(std::dynamic_pointer_cast<VectorType  >(t));
    case CompositeType::FUNCTION:return typeToString(std::dynamic_pointer_cast<FunctionType>(t));
  }
  return "";
}

std::string typeToString(std::shared_ptr<UnqualifiedType>const&t){
  switch(t->typeOfUnqualifiedType){
    case UnqualifiedType::COMPOSITE:return typeToString(std::dynamic_pointer_cast<CompositeType>(t));
    case UnqualifiedType::BIT      :return typeToString(std::dynamic_pointer_cast<BitType      >(t));
    case UnqualifiedType::ANY      :return typeToString(std::dynamic_pointer_cast<AnyType      >(t));
    case UnqualifiedType::TYPENAME :return typeToString(std::dynamic_pointer_cast<TypenameType >(t));
  }
  return "";
}

std::string typeToString(std::shared_ptr<QualifiedType>const&t){
  std::stringstream ss;
  switch(t->inOut){
    case QualifiedType::IN   :ss << "in"   ;break;
    case QualifiedType::OUT  :ss << "out"  ;break;
    case QualifiedType::INOUT:ss << "inout";break;
  }
  ss << " ";
  ss << typeToString(t->type);
  return ss.str();
}

std::string typeToString(std::shared_ptr<Type>const&t){
  switch(t->typeOfType){
    case Type::QUALIFIED  :return typeToString(std::dynamic_pointer_cast<QualifiedType  >(t));
    case Type::UNQUALIFIED:return typeToString(std::dynamic_pointer_cast<UnqualifiedType>(t));
  }
  return "";
};

int main(){
  auto functionType = std::make_shared<FunctionType>(
      std::vector<std::shared_ptr<QualifiedType>>{});
  return 0;
}
