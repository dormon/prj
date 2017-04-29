#pragma once

#include"Function.h"
#include"TypeRegister.h"

template<typename OUTPUT,typename...ARGS,std::size_t...I>
OUTPUT uber_call(Function*mf,OUTPUT(*fce)(ARGS...),std::index_sequence<I...>){
  PRINT_CALL_STACK(mf,fce);
  assert(mf!=nullptr);
  assert(fce!=nullptr);
  return fce((*mf->getInputData(I))...);
}

template<typename OUTPUT,typename CLASS,typename...ARGS,std::size_t...I>
OUTPUT uber_class_call(Function*mf,OUTPUT(CLASS::*fce)(ARGS...),std::index_sequence<I...>){
  PRINT_CALL_STACK(mf,fce);
  assert(mf!=nullptr);
  assert(fce!=nullptr);
  class Empty;
  using emptyType = OUTPUT(Empty::*)(ARGS...);
  return ((Empty*)(*mf->getInputData(0))->*((emptyType)fce))((*mf->getInputData(1+I))...);
}

template<typename OUTPUT,typename...ARGS,typename std::enable_if<!std::is_same<OUTPUT,void>::value,unsigned>::type = 0>
std::shared_ptr<Function>createFce(std::shared_ptr<TypeRegister>const&tr,OUTPUT(*impl)(ARGS...)){
  PRINT_CALL_STACK(tr,impl);
  assert(tr!=nullptr);
  class Fce: public Function{
    public:
      Fce(TypeId id,std::shared_ptr<TypeRegister>const&tr,OUTPUT(*f)(ARGS...)):Function(id,tr){
        PRINT_CALL_STACK(tr,f);
        this->_impl = f;
      }
      virtual bool _do()override{
        PRINT_CALL_STACK();
        assert(this!=nullptr);
        assert(this->_impl!=nullptr);
        (OUTPUT&)*this->getOutputData() = uber_call(this,this->_impl,typename std::make_index_sequence<sizeof...(ARGS)>{});
        //*(OUTPUT*)*this->getOutputData() = uber_call(this,this->_impl,typename std::make_index_sequence<sizeof...(ARGS)>{});
        //reinterpret_cast<OUTPUT&>(*this->getOutputData()) = uber_call(this,this->_impl,typename std::make_index_sequence<sizeof...(ARGS)>{});
        return true;
      }
    protected:
      OUTPUT(*_impl)(ARGS...);
  };
  return std::make_shared<Fce>(tr->addType<OUTPUT(ARGS...)>(),tr,impl);
}

template<typename OUTPUT,typename...ARGS,typename std::enable_if<std::is_same<OUTPUT,void>::value,unsigned>::type = 0>
std::shared_ptr<Function>createFce(std::shared_ptr<TypeRegister>const&tr,OUTPUT(*impl)(ARGS...)){
  PRINT_CALL_STACK(tr,impl);
  assert(tr!=nullptr);
  class Fce: public Function{
    public:
      Fce(TypeId id,std::shared_ptr<TypeRegister>const&tr,OUTPUT(*f)(ARGS...)):Function(id,tr){
        PRINT_CALL_STACK(tr,f);
        this->_impl = f;
      }
      virtual bool _do(){
        PRINT_CALL_STACK();
        assert(this!=nullptr);
        assert(this->_impl!=nullptr);
        uber_call(this,this->_impl,typename std::make_index_sequence<sizeof...(ARGS)>{});
        return true;
      }
    protected:
      OUTPUT(*_impl)(ARGS...);
  };
  return std::make_shared<Fce>(tr->addType<OUTPUT(ARGS...)>(),tr,impl);
}


template<typename OUTPUT,typename CLASS,typename...ARGS,typename std::enable_if<!std::is_same<OUTPUT,void>::value,unsigned>::type = 0>
std::shared_ptr<Function>createFce(std::shared_ptr<TypeRegister>const&tr,OUTPUT(CLASS::*impl)(ARGS...)){
  PRINT_CALL_STACK(tr,impl);
  assert(tr!=nullptr);
  class Fce: public Function{
    public:
      Fce(TypeId id,std::shared_ptr<TypeRegister>const&tr,OUTPUT(CLASS::*f)(ARGS...)):Function(id,tr){
        PRINT_CALL_STACK(id,tr,f);
        this->_impl = f;
      }
      virtual bool _do()override{
        PRINT_CALL_STACK();
        assert(this!=nullptr);
        assert(this->_impl!=nullptr);
        (OUTPUT&)*this->getOutputData() = uber_class_call(this,this->_impl,typename std::make_index_sequence<sizeof...(ARGS)>{});
        return true;
      }
    protected:
      OUTPUT(CLASS::*_impl)(ARGS...);
  };
  return std::make_shared<Fce>(tr->addType<OUTPUT(ARGS...)>(),tr,impl);

}


