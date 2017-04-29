#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<list>
#include<cstring>
#include<cassert>
#include<functional>
#include<typeinfo>
#include<typeindex>

template<typename FUNCTION>struct FunctionArgumentTypes;
template<typename RETURN,typename... ARGUMENTS>
struct FunctionArgumentTypes<RETURN(ARGUMENTS...)>{
  using type = std::tuple<ARGUMENTS...>;
};

template<typename FUNCTION>struct FunctionReturnType;
template<typename RETURN,typename... ARGUMENTS>
struct FunctionReturnType<RETURN(ARGUMENTS...)>{
  using type = RETURN;
};

template<typename FUNCTION>struct MemberFunctionArgumentTypes;
template<typename RETURN,typename CLASS,typename... ARGUMENTS>
struct MemberFunctionArgumentTypes<RETURN(CLASS::*)(ARGUMENTS...)>{
  using type = std::tuple<ARGUMENTS...>;
};
template<typename RETURN,typename CLASS,typename... ARGUMENTS>
struct MemberFunctionArgumentTypes<RETURN(CLASS::*)(ARGUMENTS...)const>{
  using type = std::tuple<ARGUMENTS...>;
};

template<typename FUNCTION>struct MemberFunctionClassType;
template<typename RETURN,typename CLASS,typename...ARGUMENTS>
struct MemberFunctionClassType<RETURN(CLASS::*)(ARGUMENTS...)>{
  using type = CLASS;
};
template<typename RETURN,typename CLASS,typename...ARGUMENTS>
struct MemberFunctionClassType<RETURN(CLASS::*)(ARGUMENTS...)const>{
  using type = CLASS;
};

template<typename FUNCTION>struct MemberFunctionReturnType;
template<typename RETURN,typename CLASS,typename...ARGUMENTS>
struct MemberFunctionReturnType<RETURN(CLASS::*)(ARGUMENTS...)>{
  using type = RETURN;
};
template<typename RETURN,typename CLASS,typename...ARGUMENTS>
struct MemberFunctionReturnType<RETURN(CLASS::*)(ARGUMENTS...)const>{
  using type = RETURN;
};

template<typename T,typename U,typename = int>
struct isSame: std::false_type{};

template<typename T,typename U>
struct isSame<T,U, typename std::enable_if<std::is_same<typename std::remove_cv<typename std::remove_reference<T>::type>::type,typename std::remove_cv<typename std::remove_reference<U>::type>::type>::value,int>::type>: std::true_type{};


using TypeId = size_t;

class Type{
  public:
    TypeId id;
    enum TypeType{TYPE,VOID,I8,I16,I32,I64,U8,U16,U32,U64,F32,F64,STRING,CLASS,STRUCT,POINTER,ARRAY,FUNCTION}type;
    Type(TypeId const&i,TypeType const&t):id(i),type(t){}
    virtual std::string toStr()const{
      return "";
    }
};

template<typename TYPE>
Type::TypeType type2TypeType();

template<>Type::TypeType type2TypeType<void       >(){return Type::VOID  ;}
template<>Type::TypeType type2TypeType<int8_t     >(){return Type::I8    ;}
template<>Type::TypeType type2TypeType<int16_t    >(){return Type::I16   ;}
template<>Type::TypeType type2TypeType<int32_t    >(){return Type::I32   ;}
template<>Type::TypeType type2TypeType<int64_t    >(){return Type::I64   ;}
template<>Type::TypeType type2TypeType<uint8_t    >(){return Type::U8    ;}
template<>Type::TypeType type2TypeType<uint16_t   >(){return Type::U16   ;}
template<>Type::TypeType type2TypeType<uint32_t   >(){return Type::U32   ;}
template<>Type::TypeType type2TypeType<uint64_t   >(){return Type::U64   ;}
template<>Type::TypeType type2TypeType<float      >(){return Type::F32   ;}
template<>Type::TypeType type2TypeType<double     >(){return Type::F64   ;}
template<>Type::TypeType type2TypeType<std::string>(){return Type::STRING;}

class VoidType  :public Type{public: VoidType  (TypeId const&i):Type(i,VOID  ){}};
class I8Type    :public Type{public: I8Type    (TypeId const&i):Type(i,I8    ){}};
class I16Type   :public Type{public: I16Type   (TypeId const&i):Type(i,I16   ){}};
class I32Type   :public Type{public: I32Type   (TypeId const&i):Type(i,I32   ){}};
class I64Type   :public Type{public: I64Type   (TypeId const&i):Type(i,I64   ){}};
class U8Type    :public Type{public: U8Type    (TypeId const&i):Type(i,U8    ){}};
class U16Type   :public Type{public: U16Type   (TypeId const&i):Type(i,U16   ){}};
class U32Type   :public Type{public: U32Type   (TypeId const&i):Type(i,U32   ){}};
class U64Type   :public Type{public: U64Type   (TypeId const&i):Type(i,U64   ){}};
class F32Type   :public Type{public: F32Type   (TypeId const&i):Type(i,F32   ){}};
class F64Type   :public Type{public: F64Type   (TypeId const&i):Type(i,F64   ){}};
class StringType:public Type{public: StringType(TypeId const&i):Type(i,STRING){}};

class ClassType: public Type{
  public:
    std::type_index typeIndex;
    ClassType(
        TypeId          const&i    ,
        std::type_index const&index):
      Type     (i,CLASS),
      typeIndex(index  ){}
};

class StructType: public Type{
  public:
    std::vector<std::shared_ptr<Type>>elements;
    StructType(
        TypeId                            const&i,
        std::vector<std::shared_ptr<Type>>const&e):
      Type    (i,STRUCT),
      elements(e       ){}
};

class PointerType: public Type{
  public:
    std::shared_ptr<Type>inner;
    PointerType(
        TypeId               const&i ,
        std::shared_ptr<Type>const&in):
      Type (i,POINTER),
      inner(in       ){}
};

class ArrayType: public Type{
  public:
    std::shared_ptr<Type>inner;
    size_t               size ;
    ArrayType(
        TypeId               const&i ,
        std::shared_ptr<Type>const&in,
        size_t               const&s ):
      Type (i,ARRAY),
      inner(in     ),
      size (s      ){}
};

class FunctionType: public Type{
  public:
    std::shared_ptr<Type>             returnType   ;
    std::vector<std::shared_ptr<Type>>argumentTypes;
    FunctionType(
        TypeId                            const&i   ,
        std::shared_ptr<Type>             const&rt  ,
        std::vector<std::shared_ptr<Type>>const&args):
      Type         (i,FUNCTION),
      returnType   (rt        ),
      argumentTypes(args      ){}
};

class TypeRegister{
  public:
    std::map<TypeId,std::shared_ptr<Type>>types;
    template<typename T,typename std::enable_if<isSame<T,void                    >::value,int8_t>::type = 0>TypeId addType();
    template<typename T,typename std::enable_if<isSame<T,int8_t                  >::value,int8_t>::type = 0>TypeId addType();
    template<typename T,typename std::enable_if<isSame<T,int16_t                 >::value,int8_t>::type = 0>TypeId addType();
    template<typename T,typename std::enable_if<isSame<T,int32_t                 >::value,int8_t>::type = 0>TypeId addType();
    template<typename T,typename std::enable_if<isSame<T,int64_t                 >::value,int8_t>::type = 0>TypeId addType();
    template<typename T,typename std::enable_if<isSame<T,uint8_t                 >::value,int8_t>::type = 0>TypeId addType();
    template<typename T,typename std::enable_if<isSame<T,uint16_t                >::value,int8_t>::type = 0>TypeId addType();
    template<typename T,typename std::enable_if<isSame<T,uint32_t                >::value,int8_t>::type = 0>TypeId addType();
    template<typename T,typename std::enable_if<isSame<T,uint64_t                >::value,int8_t>::type = 0>TypeId addType();
    template<typename T,typename std::enable_if<isSame<T,float                   >::value,int8_t>::type = 0>TypeId addType();
    template<typename T,typename std::enable_if<isSame<T,double                  >::value,int8_t>::type = 0>TypeId addType();
    template<typename T,typename std::enable_if<isSame<T,std::string             >::value,int8_t>::type = 0>TypeId addType();
    template<typename T,typename std::enable_if<std::is_pointer                <T>::value,int8_t>::type = 0>TypeId addType();
    template<typename T,typename std::enable_if<std::is_array                  <T>::value,int8_t>::type = 0>TypeId addType();
    template<typename T,typename std::enable_if<std::is_function               <T>::value,int8_t>::type = 0>TypeId addType();
    template<typename T,typename std::enable_if<std::is_member_function_pointer<T>::value,int8_t>::type = 0>TypeId addType();
    template<typename T,typename std::enable_if<std::is_class                  <T>::value && !isSame<T,std::string>::value,int8_t>::type = 0>TypeId addType();
    TypeId addPointer    (
        TypeId             const&innerType    );
    TypeId addArray      (
        TypeId             const&innerType    ,
        size_t             const&arraySize    );
    TypeId addStruct     (
        std::vector<TypeId>const&elementTypes );
    TypeId addFunction   (
        TypeId             const&returnType   ,
        std::vector<TypeId>const&argumentTypes);
    TypeId addMemFunction(
        TypeId             const&returnType   ,
        TypeId             const&classType    ,
        std::vector<TypeId>const&argumentTypes);

    template<typename TUPLE,std::size_t...I>
      inline std::vector<TypeId>tupleToTypeIds2_help(std::index_sequence<I...>){
        assert(this!=nullptr);
        return {this->addType<typename std::tuple_element<I,TUPLE>::type>()...};
      }
    template<typename TUPLE>
      inline std::vector<TypeId>tupleToTypeIds2(){
        assert(this!=nullptr);
        return this->tupleToTypeIds2_help<TUPLE>(typename std::make_index_sequence<std::tuple_size<TUPLE>::value>{});
      }
};

#define TYPEREGISTER_ADDTYPE(TYPETYPE,TYPECLASS)\
  assert(this!=nullptr);\
for(auto const&x:this->types)\
if(x.second->type == Type::TYPETYPE)return x.first;\
auto id = this->types.size();\
this->types[id]=std::make_shared<TYPECLASS>(id);\
return id

template<>TypeId TypeRegister::addType<void              >(){TYPEREGISTER_ADDTYPE(VOID  ,VoidType  );}
template<>TypeId TypeRegister::addType<int8_t            >(){TYPEREGISTER_ADDTYPE(I8    ,I8Type    );}
template<>TypeId TypeRegister::addType<int16_t           >(){TYPEREGISTER_ADDTYPE(I16   ,I16Type   );}
template<>TypeId TypeRegister::addType<int32_t           >(){TYPEREGISTER_ADDTYPE(I32   ,I32Type   );}
template<>TypeId TypeRegister::addType<int64_t           >(){TYPEREGISTER_ADDTYPE(I64   ,I64Type   );}
template<>TypeId TypeRegister::addType<uint8_t           >(){TYPEREGISTER_ADDTYPE(U8    ,U8Type    );}
template<>TypeId TypeRegister::addType<uint16_t          >(){TYPEREGISTER_ADDTYPE(U16   ,U16Type   );}
template<>TypeId TypeRegister::addType<uint32_t          >(){TYPEREGISTER_ADDTYPE(U32   ,U32Type   );}
template<>TypeId TypeRegister::addType<uint64_t          >(){TYPEREGISTER_ADDTYPE(U64   ,U64Type   );}
template<>TypeId TypeRegister::addType<float             >(){TYPEREGISTER_ADDTYPE(F32   ,F32Type   );}
template<>TypeId TypeRegister::addType<double            >(){TYPEREGISTER_ADDTYPE(F64   ,F64Type   );}
template<>TypeId TypeRegister::addType<std::string       >(){TYPEREGISTER_ADDTYPE(STRING,StringType);}
template<>TypeId TypeRegister::addType<void        const >(){TYPEREGISTER_ADDTYPE(VOID  ,VoidType  );}
template<>TypeId TypeRegister::addType<int8_t      const >(){TYPEREGISTER_ADDTYPE(I8    ,I8Type    );}
template<>TypeId TypeRegister::addType<int16_t     const >(){TYPEREGISTER_ADDTYPE(I16   ,I16Type   );}
template<>TypeId TypeRegister::addType<int32_t     const >(){TYPEREGISTER_ADDTYPE(I32   ,I32Type   );}
template<>TypeId TypeRegister::addType<int64_t     const >(){TYPEREGISTER_ADDTYPE(I64   ,I64Type   );}
template<>TypeId TypeRegister::addType<uint8_t     const >(){TYPEREGISTER_ADDTYPE(U8    ,U8Type    );}
template<>TypeId TypeRegister::addType<uint16_t    const >(){TYPEREGISTER_ADDTYPE(U16   ,U16Type   );}
template<>TypeId TypeRegister::addType<uint32_t    const >(){TYPEREGISTER_ADDTYPE(U32   ,U32Type   );}
template<>TypeId TypeRegister::addType<uint64_t    const >(){TYPEREGISTER_ADDTYPE(U64   ,U64Type   );}
template<>TypeId TypeRegister::addType<float       const >(){TYPEREGISTER_ADDTYPE(F32   ,F32Type   );}
template<>TypeId TypeRegister::addType<double      const >(){TYPEREGISTER_ADDTYPE(F64   ,F64Type   );}
template<>TypeId TypeRegister::addType<std::string const >(){TYPEREGISTER_ADDTYPE(STRING,StringType);}
template<>TypeId TypeRegister::addType<int8_t      const&>(){TYPEREGISTER_ADDTYPE(I8    ,I8Type    );}
template<>TypeId TypeRegister::addType<int16_t     const&>(){TYPEREGISTER_ADDTYPE(I16   ,I16Type   );}
template<>TypeId TypeRegister::addType<int32_t     const&>(){TYPEREGISTER_ADDTYPE(I32   ,I32Type   );}
template<>TypeId TypeRegister::addType<int64_t     const&>(){TYPEREGISTER_ADDTYPE(I64   ,I64Type   );}
template<>TypeId TypeRegister::addType<uint8_t     const&>(){TYPEREGISTER_ADDTYPE(U8    ,U8Type    );}
template<>TypeId TypeRegister::addType<uint16_t    const&>(){TYPEREGISTER_ADDTYPE(U16   ,U16Type   );}
template<>TypeId TypeRegister::addType<uint32_t    const&>(){TYPEREGISTER_ADDTYPE(U32   ,U32Type   );}
template<>TypeId TypeRegister::addType<uint64_t    const&>(){TYPEREGISTER_ADDTYPE(U64   ,U64Type   );}
template<>TypeId TypeRegister::addType<float       const&>(){TYPEREGISTER_ADDTYPE(F32   ,F32Type   );}
template<>TypeId TypeRegister::addType<double      const&>(){TYPEREGISTER_ADDTYPE(F64   ,F64Type   );}
template<>TypeId TypeRegister::addType<std::string const&>(){TYPEREGISTER_ADDTYPE(STRING,StringType);}



template<typename T,typename std::enable_if<std::is_class<T>::value && !isSame<T,std::string>::value,int8_t>::type>TypeId TypeRegister::addType(){
  assert(this!=nullptr);
  for(auto const&x:this->types)
    if(x.second->type == Type::CLASS){
      auto const p = std::dynamic_pointer_cast<ClassType>(x.second);
      if(p->typeIndex == typeid(T))
        return x.first;
    }
  auto const id = this->types.size();
  this->types[id] = std::make_shared<ClassType>(id,typeid(T));
  return id;
}

template<typename T,typename std::enable_if<std::is_pointer<T>::value,int8_t>::type>TypeId TypeRegister::addType(){
  assert(this!=nullptr);
  auto const innerType = this->addType<typename std::remove_pointer<T>::type>();
  return this->addPointer(innerType);
}

template<typename TYPE,typename std::enable_if<std::is_array<TYPE>::value,int8_t>::type>
TypeId TypeRegister::addType(){
  assert(this != nullptr);
  using INNER_TYPE = typename std::remove_extent<TYPE>::type;
  auto const innerTypeID = this->addType<INNER_TYPE>();
  auto const arraySize   = std::extent<TYPE>::value;
  return this->addArray(innerTypeID,arraySize);
}

template<typename TYPE,typename std::enable_if<std::is_function<TYPE>::value,int8_t>::type>
TypeId TypeRegister::addType(){
  assert(this != nullptr);
  using RETURN_TYPE = typename FunctionReturnType   <TYPE>::type;
  using ARGS_TYPES  = typename FunctionArgumentTypes<TYPE>::type;
  auto const returnTypeID    = this->addType<RETURN_TYPE>();
  auto const argumentTypeIDs = this->tupleToTypeIds2<ARGS_TYPES>();
  return this->addFunction(returnTypeID,argumentTypeIDs);
}

template<typename T,typename std::enable_if<std::is_member_function_pointer<T>::value,int8_t>::type>
TypeId TypeRegister::addType(){
  assert(this!=nullptr);
  using RETURN_TYPE    = typename MemberFunctionReturnType   <T>::type;
  using CLASS_TYPE     = typename MemberFunctionClassType    <T>::type;
  using ARGUMENT_TYPES = typename MemberFunctionArgumentTypes<T>::type;
  auto const rt   = this->addType<RETURN_TYPE>();
  auto const cl   = this->addType<CLASS_TYPE>();
  auto const args = this->tupleToTypeIds2<ARGUMENT_TYPES>();
  return this->addMemFunction(rt,cl,args);
}

TypeId TypeRegister::addPointer    (
    TypeId             const&innerType    ){
  assert(this != nullptr);
  for(auto const&x:this->types)
    if(x.second->type == Type::POINTER){
      auto const p = std::dynamic_pointer_cast<PointerType const>(x.second);
      if(p->inner->id == innerType)
        return x.first;
    }
  auto const id = this->types.size();
  this->types[id] = std::make_shared<PointerType>(id,this->types.at(innerType));
  return id;
}

TypeId TypeRegister::addArray      (
    TypeId             const&innerType    ,
    size_t             const&arraySize    ){
  assert(this != nullptr);
  for(auto const&x:this->types)
    if(x.second->type == Type::ARRAY){
      auto const p = std::dynamic_pointer_cast<ArrayType const>(x.second);
      if(p->inner->id == innerType)
        return x.first;
    }
  auto const id = this->types.size();
  this->types[id] = std::make_shared<ArrayType>(id,this->types.at(innerType),arraySize);
  return id;
}

TypeId TypeRegister::addStruct     (
    std::vector<TypeId>const&elementTypes ){
  assert(this != nullptr);
  for(auto const&x:this->types)
    if(x.second->type == Type::STRUCT){
      auto const p = std::dynamic_pointer_cast<StructType const>(x.second);
      if(p->elements.size() != elementTypes.size())continue;
      bool different = false;
      for(size_t i=0;i<elementTypes.size();++i)
        if(p->elements.at(i)->id != elementTypes.at(i)){
          different = true;
          break;
        }
      if(different)continue;
      return x.first;
    }
  std::vector<std::shared_ptr<Type>>sargs;
  for(auto const&x:elementTypes)
    sargs.push_back(this->types.at(x));

  auto const id = this->types.size();
  this->types[id] = std::make_shared<StructType>(id,sargs);
  return id;
}

TypeId TypeRegister::addFunction(
    TypeId             const&returnType   ,
    std::vector<TypeId>const&argumentTypes){
  assert(this!=nullptr);
  for(auto const&t:this->types){
    if(t.second->type == Type::FUNCTION){
      auto const p = std::dynamic_pointer_cast<FunctionType>(t.second);
      if(p->returnType->id != returnType)continue;
      if(p->argumentTypes.size() != argumentTypes.size())continue;
      bool different = false;
      for(size_t i=0;i<argumentTypes.size();++i)
        if(p->argumentTypes.at(i)->id != argumentTypes.at(i)){
          different = true;
          break;
        }
      if(different)continue;
      return t.first;
    }
  }
  std::vector<std::shared_ptr<Type>>sargs;
  for(auto const&x:argumentTypes)
    sargs.push_back(this->types.at(x));

  auto const id = this->types.size();
  this->types[id] = std::make_shared<FunctionType>(id,this->types.at(returnType),sargs);
  return id;
}

TypeId TypeRegister::addMemFunction(
    TypeId             const&returnType   ,
    TypeId             const&classType    ,
    std::vector<TypeId>const&argumentTypes){
  assert(this!=nullptr);
  for(auto const&t:this->types)
    if(t.second->type == Type::FUNCTION){
      auto const p = std::dynamic_pointer_cast<FunctionType>(t.second);
      if(p->argumentTypes.size() != argumentTypes.size()+1)continue;
      if(p->returnType->id != returnType)continue;
      if(p->argumentTypes.at(0)->id != classType)continue;
      bool different = false;
      for(size_t i=0;i<argumentTypes.size();++i)
        if(p->argumentTypes[i+1]->id != argumentTypes[i]){
          different = true;
          break;
        }
      if(different)continue;
      return t.first;
    }
  auto const id = this->types.size();
  std::vector<std::shared_ptr<Type>>sargs;
  for(auto const&x:argumentTypes)
    sargs.push_back(this->types.at(x));

  this->types[id] = std::make_shared<FunctionType>(id,this->types.at(returnType),sargs);
  return id;
}




using VoidFunctionType = void(*)();

using FunctionId = size_t;

class FunctionNode{
  public:
    FunctionId                                id     ;
    std::vector<std::shared_ptr<FunctionNode>>inputs ;
    std::vector<FunctionNode*>                outputs;
};

class Function{
  public:
    FunctionId  id  ;
    TypeId      type;
    std::string name;
    Function(
        FunctionId  const&i,
        TypeId      const&t,
        std::string const&n):
      id  (i),
      type(t),
      name(n){}
    virtual ~Function(){}
};

class AtomicFunction: public Function{
  public:
    VoidFunctionType ptr ;
    AtomicFunction(
        FunctionId       const&i,
        TypeId           const&t,
        std::string      const&n,
        VoidFunctionType const&p):
      Function(i,t,n),
      ptr     (p    ){}
};

class CompositeFunction: public Function{
  public:
    using Input = std::tuple<std::shared_ptr<FunctionNode>,size_t>;
    enum InputParts{
      FUNCTION = 0,
      INPUT    = 1,
    };
    std::shared_ptr<FunctionNode>root  ;
    std::vector<Input>           inputs;
    CompositeFunction(
        FunctionId                   const&i ,
        TypeId                       const&t ,
        std::string                  const&n ,
        std::shared_ptr<FunctionNode>const&r ,
        std::vector<Input>           const&in):
      Function(i,t,n),
      root    (r    ),
      inputs  (in   ){}
};

class FunctionRegister{
  public:
    std::vector<std::shared_ptr<Function>>functions;
    template<typename RETURN,typename...ARGUMENTS>
      FunctionId addFunction(
          std::string const&name,
          RETURN(*fce)(ARGUMENTS...),
          std::shared_ptr<TypeRegister>const&tr){
        assert(this!=nullptr);
        assert(tr!=nullptr);
        FunctionId id = this->functions.size();
        this->functions.push_back(std::make_shared<AtomicFunction>(id,tr->addType<RETURN(*)(ARGUMENTS...)>(),name,fce));
      }
    FunctionId addFunction(
        std::string                          const&name  ,
        std::shared_ptr<FunctionNode>        const&root  ,
        std::vector<CompositeFunction::Input>const&inputs,
        std::shared_ptr<TypeRegister>        const&tr    ){
      assert(this!=nullptr);
      assert(tr!=nullptr);
      FunctionId const id         = this->functions.size();
      TypeId     const returnType = this->functions.at(root->id)->type;
      std::vector<TypeId> argumentTypes;
      for(size_t i=0;i<inputs.size();++i){
        auto const&input         = inputs.at(i);
        auto const&inputFunction = std::get<CompositeFunction::FUNCTION>(input)->id;
        auto const&inputNumber   = std::get<CompositeFunction::INPUT   >(input)    ;
        auto const p             = std::dynamic_pointer_cast<FunctionType>(this->functions.at(inputFunction));
        argumentTypes.push_back(p->argumentTypes.at(inputNumber)->id);
      }
      auto const type = tr->addFunction(returnType,argumentTypes);
      this->functions.push_back(std::make_shared<CompositeFunction>(id,type,name,root,inputs));
      return id;
    }
};

class Formula{
  public:
    virtual ~Formula(){}
};

class Term: public Formula{
  public:
    std::string name;
};

class Negation: public Formula{
  public:
    std::shared_ptr<Formula>formula;
};

class Implication: public Formula{
  public:
    std::shared_ptr<Formula>left ;
    std::shared_ptr<Formula>right;
};

bool bool_impl(bool const&a,bool const&b){return !a||b;}
bool bool_neg (bool const&a             ){return !a   ;}

// Syntax:
//
// Formula         : TermFormula
// Formula         : CompositeFormula
// TermFormula     : identifier
// TermFormula     : ~Formula
// TermFormula     : ( CompositeFormula )
// CompositeFormula: Formula -> Formula
//
// I0,I1,I2,Ii,... are shortcuts for identifiers
// F0,F1,F2,Fi,... are shortcuts for formulas
// _ is any Ii,Fi
//
//
// Axioms:
// F0 -> (F1 -> F0)
// (~F1 -> ~ F0) -> (F0 -> F1)
// (F0 -> (F1 -> F2)) -> ((F0 -> F1) -> (F0 -> F2))
//
// Deriving rule:
// O(F0,F0->F1) = F1
// 
// Substitution rule:
// S(old,where,new)
// S(I0,I0    ,F0) == F0
// S(I0,F0->F1,F2) == S(I0,F0,F2) -> S(I0,F1,F2)
// S(I0,(F0)  ,F1) == (S(I0,F0,F1))
// S(I0,~F0   ,F1) == ~S(I0,F0,F1)
// S(F0,F0    ,F1) == F1
// S(F0,F1->F2,F3) == S(F0,F1,F3) -> S(F0,F2,F3)
// S(F0,(F0)  ,F1) == (F1)
// S(F0,~F0   ,F1) == ~F1
// S(_ ,F0    ,_ ) == F0
//
// Ii == Ii
// Fi == Fi
//
//

class Class{
  public:
    void set(int32_t const&v){
      assert(this!=nullptr);
      this->a = v;
    }
    int32_t get()const{
      assert(this!=nullptr);
      return this->a;
    }
  protected:
    int32_t a = 0;
};


int main(){
  TypeRegister a;
  a.addType<void>();
  a.addType<float>();
  std::cout<<"int and int is: "      <<isSame<int,int       >::value<<std::endl;
  std::cout<<"int and float is: "    <<isSame<int,float     >::value<<std::endl;
  std::cout<<"int and int const is: "<<isSame<int,int const >::value<<std::endl;
  std::cout<<"int and int const&:   "<<isSame<int,int const&>::value<<std::endl;
  std::cout<<"int* and int*const:   "<<isSame<int*,int*const>::value<<std::endl;
  std::cout<<"int* and int const*const:   "<<isSame<int*,int const*const>::value<<std::endl;
  std::cout<<"int* and int const*:   "<<isSame<int*,int const*>::value<<std::endl;
  //a.addTyoe<float const>();
  a.addType<float*const>();
  a.addType<int32_t>();
  a.addType<int32_t const>();
  a.addType<int32_t const&>();
  a.addType<int32_t*>();
  a.addType<int32_t[3]>();
  a.addType<float[3][3]>();
  a.addType<void(int32_t)>();
  a.addType<Class>();
  a.addType<void(Class::*)(int32_t)>();
  a.addType<void(Class::*)(int32_t)const>();
  a.addType<void(Class::*)(int32_t const&)const>();
  return 0;
}
