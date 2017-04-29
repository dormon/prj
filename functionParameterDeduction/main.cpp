#include<iostream>
#include<sstream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
//#include<utility>
#include<geCore/dtemplates.h>
#include<geCore/functionRegister.h>
#include<geCore/function.h>
#include<geCore/stdFunctions.h>

template<typename TYPE>std::string name();
template<typename...ARGS>std::string names();
template<typename HEAD,typename...ARGS>std::string names_helper(){return name<HEAD>()+((sizeof...(ARGS)>0)?",":"")+names<ARGS...>();}
template<typename...ARGS>std::string names(){return names_helper<ARGS...>();}
template<>std::string names(){return std::string("");}
template<template<typename...>class CLASS>std::string className();
template<template<typename...>class CLASS,typename... ARGS>std::string name(){return className<CLASS>()+"<"+names<ARGS...>()+">";}
template<>std::string name<void       >(){return "void"    ;}
template<>std::string name<bool       >(){return "bool"    ;}
template<>std::string name<int8_t     >(){return "int8_t"  ;}
template<>std::string name<int16_t    >(){return "int16_t" ;}
template<>std::string name<int32_t    >(){return "int32_t" ;}
template<>std::string name<int64_t    >(){return "int64_t" ;}
template<>std::string name<uint8_t    >(){return "uint8_t" ;}
template<>std::string name<uint16_t   >(){return "uint16_t";}
template<>std::string name<uint32_t   >(){return "uint32_t";}
template<>std::string name<uint64_t   >(){return "uint64_t";}
template<>std::string name<float      >(){return "float"   ;}
template<>std::string name<double     >(){return "double"  ;}
template<>std::string name<std::string>(){return "string"  ;}



template<typename OUTPUT,typename...ARGS>
void deductor(OUTPUT(*FCE)(ARGS...)){
  std::cout<<names<OUTPUT,ARGS...>()<<std::endl;
}
/*
namespace ge{
  namespace core{
    template<size_t... _Indexes>
      struct _Index_tuple
      {
        typedef _Index_tuple<_Indexes..., sizeof...(_Indexes)> __next;
      };

    template<size_t _Num>
      struct _Build_index_tuple
      {
        typedef typename _Build_index_tuple<_Num - 1>::__type::__next __type;
      };

    template<>
      struct _Build_index_tuple<0>
      {
        typedef _Index_tuple<> __type;
      };

    /// Class template integer_sequence
    template<typename _Tp, _Tp... _Idx>
      struct integer_sequence
      {
        typedef _Tp value_type;
        static //constexpr
          size_t size() { return sizeof...(_Idx); }
      };

    template<typename _Tp, _Tp _Num,
      typename _ISeq = typename _Build_index_tuple<_Num>::__type>
        struct _Make_integer_sequence;

    template<typename _Tp, _Tp _Num,  size_t... _Idx>
      struct _Make_integer_sequence<_Tp, _Num, _Index_tuple<_Idx...>>
      {
        static_assert( _Num >= 0,
            "Cannot make integer sequence of negative length" );

        typedef integer_sequence<_Tp, static_cast<_Tp>(_Idx)...> __type;
      };

    /// Alias template make_integer_sequence
    template<typename _Tp, _Tp _Num>
      using make_integer_sequence
      = typename _Make_integer_sequence<_Tp, _Num>::__type;

    /// Alias template index_sequence
    template<size_t... _Idx>
      using index_sequence = integer_sequence<size_t, _Idx...>;

    /// Alias template make_index_sequence
    template<size_t _Num>
      using make_index_sequence = make_integer_sequence<size_t, _Num>;

    /// Alias template index_sequence_for
    template<typename... _Types>
      using index_sequence_for = make_index_sequence<sizeof...(_Types)>;
  }
}

template<typename OUTPUT,typename...ARGS>
inline std::vector<ge::core::TypeRegister::DescriptionElement>getDescription(
    std::shared_ptr<ge::core::TypeRegister>const&tr,
    OUTPUT(*FCE)(ARGS...)){
  std::vector<ge::core::TypeRegister::DescriptionElement>result;
  result.push_back(ge::core::TypeRegister::FCE);
  result.push_back(tr->getTypeId(ge::core::TypeRegister::getTypeKeyword<OUTPUT>()));
  result.push_back(sizeof...(ARGS));
  std::vector<ge::core::TypeRegister::DescriptionElement> ar(tr->getTypeId(ge::core::TypeRegister::getTypeKeyword<ARGS>())...);
  for(auto x:ar)result.push_back(x);
  return result;
}

template<typename OUTPUT,typename...ARGS,std::size_t...I>
OUTPUT uber_call(Function*mf,OUTPUT(*FCE)(ARGS...),ge::core::index_sequence<I...>){
  return FCE((ARGS&)(*mf->getInputData(I))...);
}

template<typename OUTPUT,typename...ARGS>
void registerBasicFunction(
    std::shared_ptr<ge::core::FunctionRegister>const&fr,
    const std::string name,
    OUTPUT(*FCE)(ARGS...)){
  static const std::string ss=name;
  auto tr = fr->getTypeRegister();
  auto tid = tr->addType("",getDescription(tr,FCE));

  class BasicFunction: public ge::core::AtomicFunction{
    public:
      BasicFunction(std::shared_ptr<ge::core::FunctionRegister>const&f,FunctionRegister::FunctionID id):AtomicFunction(f,id){}
      virtual ~BasicFunction(){}
    protected:
      virtual bool _do(){
        typedef OUTPUT(*FF)(ARGS...);
        FF f=reinterpret_cast<FF>(this->_functionRegister->getImplementation(this->_id));
        (OUTPUT&)(*this->getOutputData()) = uber_call<OUTPUT>(this,f,ge::core::make_index_sequence<sizeof...(ARGS)>());
        return true;
      }
  };
  auto f=fr->addFunction(tid,name,ge::core::Function::factory<BasicFunction>(name));
  fr->addImplementation(f,reinterpret_cast<void*>(FCE));
}

#define DEFINE_FUNCTION_TYPE_KEYWORD(fce,name)\
namespace ge{\
  namespace core{\
    template<>std::string TypeRegister::getTypeKeyword<decltype(&fce)>(){return name;}\
  }\
}*/


int blb(int a,int b){
  return a+b;
}

int baba(int a,int b){
  return a*b;
}

//DEFINE_FUNCTION_TYPE_KEYWORD(blb,"blb_type")


int main(){
  auto tr = std::make_shared<ge::core::TypeRegister>();
  auto fr = std::make_shared<ge::core::FunctionRegister>(tr);
  ge::core::registerStdFunctions(fr);

  registerBasicFunction(fr,"blb",blb);
  registerBasicFunction(fr,"baba",baba);

  auto f=fr->sharedFunction("blb");
  auto ff=std::dynamic_pointer_cast<Function>(f);
  ff->bindOutput(tr->sharedAccessor("i32"));
  auto a=std::make_shared<ge::core::Nullary>(fr,(int32_t)10);
  auto b=std::make_shared<ge::core::Nullary>(fr,(int32_t)12);
  ff->bindInput(0,a);
  ff->bindInput(1,b);
  (*f)();
  std::cout<<(int32_t)(*ff->getOutputData())<<std::endl;

  auto f1=fr->sharedFunction("baba");
  auto ff1=std::dynamic_pointer_cast<Function>(f1);
  ff1->bindOutput(tr->sharedAccessor("i32"));
  auto a1=std::make_shared<ge::core::Nullary>(fr,(int32_t)10);
  auto b1=std::make_shared<ge::core::Nullary>(fr,(int32_t)12);
  ff1->bindInput(0,a1);
  ff1->bindInput(1,b1);
  (*f1)();
  std::cout<<(int32_t)(*ff1->getOutputData())<<std::endl;


  return 0;
}
