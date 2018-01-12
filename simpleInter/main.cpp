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

//
// using add <typ = ( typename ARG_A a
//
// any  -> no restrictions
// void -> all restrictions
//
// any  storage can    store anythings
// void storage cannot store anythings
//
//              any
//          _____|_____________________________________
//         |       |      |        |        |          |
//       float   uint32  bool {TYPE_LIST} [TYPE] (TYPE_LIST)->TYPE
//
// TYPE                  ::= DECLARED_TYPE | ATOMIC_TYPE | STRUCT_TYPE | ARRAY_TYPE | FUNCTION_TYPE | PROCEDURE_TYPE | DECLARED_TYPE
// DECLARED_TYPE         ::= IDENTIFIER
// ATOMIC_TYPE           ::= any | void | float | uint32_t | bool
// STRUCT_TYPE           ::= { TYPE_LIST }
// ARRAY_TYPE            ::= [ TYPE ]
// FUNCTION_TYPE         ::= ( TYPE_LIST ) -> TYPE
// PROCEDURE_TYPE        ::= ( TYPE_LIST )
// TYPE_LIST             ::= TYPE
// TYPE_LIST             ::= TYPE | TYPE_LIST
//
// STATEMENT             ::= DECLARATION | EXPRESSION | SCOPE | IF | WHILE
//
// DECLARATION           ::= VARIABLE_DECLARATION | TYPE_DECLARATION | FUNCTION_DECLARATION
// 
// VARIABLE_DECLARATION  ::= TYPE VARIABLE_NAME ;
// TYPE_DECLARATION      ::= using TYPE_NAME      = TYPE ;
// FUNCTION_DECLARATION  ::= using FUNCTION_NAME  = ( ARGUMENT_DECLARATION_LIST ) -> TYPE UNNAMED_SCOPE
//
// SCOPE                 ::= NAMED_SCOPE | UNNAMED_SCOPE
//
// NAMED_SCOPE           ::= SCOPE_NAME { STATEMENT_LIST }
// UNNAMED_SCOPE         ::=            { STATEMENT_LIST }
//
// IF                    ::= if    BOOL_EXPRESSION    UNNAMED_SCOPE
// WHILE                 ::= while BOOL_VARIABLE_NAME UNNAMED_SCOPE
//
//
//
//
//


class Type{
  public:
};

class Resource{
  public:
    virtual ~Resource(){}
};

class StructResource: public Resource{
  public:
    virtual ~StructResource(){}
    std::vector<std::shared_ptr<Resource>>resources;
};

template<typename T>
class AtomicResource: public Resource{
  public:
    virtual ~AtomicResource(){}
    AtomicResource(T d):data(d){}
    operator T(){
      return data;
    }
    T data;
};

class VectorResource: public Resource{
  public:
    virtual ~VectorResource(){}
    VectorResource(){}
    std::vector<std::shared_ptr<Resource>>data;
};



class Statement{
  public:
    virtual ~Statement(){}
    virtual void operator()() = 0;
};

class Function: public Statement{
  public:
    virtual ~Function(){}
    virtual void operator()() = 0;
};

class Body: public Statement{
  public:
    virtual ~Body(){}
    virtual void operator()()override{
      for(auto const&x:statements)
        (*x)();
    }
    std::vector<std::shared_ptr<Statement>>statements;
};

class If: public Statement{
  public:
    virtual ~If(){}
    virtual void operator()()override{
      if(static_cast<bool>((*condition)))
        (*statement)();
    }
    std::shared_ptr<AtomicResource<bool>>condition;    
    std::shared_ptr<Statement>statement;
};

class While: public Statement{
  public:
    virtual ~While(){}
    virtual void operator()()override{
      while(static_cast<bool>((*condition)))
        (*statement)();
    }
    std::shared_ptr<AtomicResource<bool>>condition;    
    std::shared_ptr<Statement>statement;
};

class AtomicFunction: public Function{
  public:
    virtual ~AtomicFunction(){}
    virtual void operator()() = 0;
    std::vector<std::shared_ptr<Resource>>inputs;
    std::shared_ptr<Resource>output;
};

template<typename RETURN,typename...ARGUMENTS>
using FunctionPointer = RETURN(*)(ARGUMENTS...);

template<typename...ARGUMENTS,std::size_t...I>
void callFunctionPointer(
    FunctionPointer<void,ARGUMENTS...>    const&fce   ,
    std::shared_ptr<Resource>             const&      ,
    std::vector<std::shared_ptr<Resource>>const&inputs,
    std::index_sequence<I...>             const&      ){
  assert(fce != nullptr);
  for(auto const&x:inputs){
    assert(x != nullptr);
    static_cast<void>(x);
  }


  fce((*std::dynamic_pointer_cast<AtomicResource<ARGUMENTS>>(inputs.at(I)))...);
}

template<
  typename        RETURN   ,
  typename    ... ARGUMENTS,
  std::size_t ... I        ,
  typename        std::enable_if<!std::is_same<RETURN,void>::value,char>::type = 0>
void callFunctionPointer(
    FunctionPointer<RETURN,ARGUMENTS...>  const&fce   ,
    std::shared_ptr<Resource>             const&output,
    std::vector<std::shared_ptr<Resource>>const&inputs,
    std::index_sequence<I...>             const&){
  assert(fce != nullptr);
  assert(output != nullptr);
  for(auto const&x:inputs){
    assert(x != nullptr);
    static_cast<void>(x);
  }

  auto const o = std::dynamic_pointer_cast<AtomicResource<RETURN>>(output);
  *o = fce((*std::dynamic_pointer_cast<AtomicResource<ARGUMENTS>>(inputs.at(I)))...);
}


template<typename RETURN,typename...ARGUMENTS>
std::shared_ptr<AtomicFunction>createFunction(FunctionPointer<RETURN,ARGUMENTS...>const&fce){

  if(fce == nullptr)
    throw std::invalid_argument{"function cannot be nullptr"};

  constexpr size_t nofArguments = sizeof...(ARGUMENTS);

  class Implementation: public AtomicFunction{
    public:
      virtual void operator()()override{
        callFunctionPointer(
            ptr,
            output,
            inputs,
            std::make_index_sequence<nofArguments>{});
      }
      FunctionPointer<RETURN,ARGUMENTS...>ptr;
  };

  auto result = std::make_shared<Implementation>();

  if(result == nullptr)
    throw std::runtime_error{"cannot allocate function instance"};

  result->ptr = fce;
  result->inputs.resize(nofArguments);

  return result;
}

template<typename T>
struct RawPointerDecorator{
  using type = T*;
};

template<typename T>
struct SharedPointerDecorator{
  using type = std::shared_ptr<T>;
};


template<
  template<typename...>class CONTAINER        , 
  template<typename   >class POINTER_DECORATOR>
class Node{
  public:
    CONTAINER<typename POINTER_DECORATOR<Node>::type>nodes;
};

template<
  template<typename...>class PARENT_CONTAINER        ,
  template<typename   >class PARENT_POINTER_DECORATOR,
  template<typename...>class CHILD_CONTAINER         ,
  template<typename   >class CHILD_POINTER_DECORATOR >
class DirectedNode: public Node<CHILD_CONTAINER,CHILD_POINTER_DECORATOR>{
  public:
    using NodeType = DirectedNode<
      PARENT_CONTAINER        ,
      PARENT_POINTER_DECORATOR,
      CHILD_CONTAINER         ,
      CHILD_POINTER_DECORATOR >;
    PARENT_CONTAINER<typename PARENT_POINTER_DECORATOR<NodeType>::type>parents;
};

class DAGNode: public DirectedNode<std::set,RawPointerDecorator,std::vector,SharedPointerDecorator>{
  public:

};

float add(float a,float b){
  return a+b;
}

void neco(){}

template<typename TYPE>
using FCE = std::function<TYPE()>;

#define DEFINE_BINARY_OPERATOR(oper,type) FCE<type> operator oper(FCE<type> const&a,FCE<type> const&b){return [&](){return a() oper b();};}

DEFINE_BINARY_OPERATOR(+,float);
DEFINE_BINARY_OPERATOR(-,float);
DEFINE_BINARY_OPERATOR(*,float);
DEFINE_BINARY_OPERATOR(/,float);

template<typename TYPE>
FCE<TYPE> constant(TYPE a){return [&](){return a;};}

template<typename TYPE>
FCE<TYPE> variable(TYPE*a){return [&](){return*a;};}

using ClosedFunction = std::function<void()>;
template<typename...ARGS>
using VoidFunction = std::function<void(ARGS...)>;

template<typename...FUNCTION_ARGUMENT_TYPES,typename...ARGUMENT_TYPES>
ClosedFunction bind(VoidFunction<FUNCTION_ARGUMENT_TYPES...>const&fce,ARGUMENT_TYPES const&... arguments){
  return [&](){fce(arguments...);};
}

int main(){
  //auto f = createFunction(&add);
  //static_cast<void>(f);


  std::cout << (constant(1)+constant(2))() << std::endl;
  Node<std::vector,SharedPointerDecorator>root;
  return 0;
}


