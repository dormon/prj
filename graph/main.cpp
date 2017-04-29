#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include"containerOfShared.h"

template<typename TYPE>
class SetOfShared: public ContainerOfShared<std::set,TYPE>{
  public:
    void add(const std::shared_ptr<TYPE>&value){
      this->insert(value);
    }
    template<typename...ARGS>
    void add(const std::shared_ptr<TYPE>&value,ARGS...args){
      this->add(value);
      this->add(args...);
    }
};

template<typename NODE>
class Child: public SetOfShared<NODE>{};

template<typename NODE>
class Parent: public SetOfShared<NODE>{};

template<typename CHILDNODE,typename PARENTNODE>
class ChildParentNode: public Child<CHILDNODE>, public Parent<PARENTNODE>{};

class Data;
class Action: public ChildParentNode<Data,Data>{
  public:
    virtual void operator()()=0;
};
class Data: public ChildParentNode<Action,Action>{};



template<typename TYPE>
class BaseData: public Data{
  protected:
    TYPE _value;
  public:
    inline BaseData(){};
    inline BaseData(const TYPE&value){this->set(value);}
    TYPE&get(){return this->_value;}
    void set(const TYPE&value){this->_value=value;}
};

template<typename RETURN,typename...INPUTS>
class Operator: public Action{


};

template<typename TYPE>
class BaseAddition: public Action{
  public:
    virtual void operator()(){
      TYPE result=0;
      for(Action::Parent::iterator ii=this->Parent::begin();ii!=this->Parent::end();++ii)
        result=result+std::static_pointer_cast<BaseData<TYPE>>(*ii)->get();
      for(Action::Child::iterator ii=this->Child::begin();ii!=this->Child::end();++ii)
        std::static_pointer_cast<BaseData<TYPE>>(*ii)->set(result);
    }
};


template<typename INPUT,typename OUTPUT>
class Cast: public Action{
  public:
    virtual void operator()();//PLUGINS
};

template<typename TYPE>
using SharedData=typename std::shared_ptr<BaseData<TYPE>>;

template<typename TYPE,typename...ARGS>
inline SharedData<TYPE> SharedDataConstructor(ARGS... args){
  return std::make_shared<BaseData<TYPE>>(args...);
}


int main(){
  SharedData<float>value0  = SharedDataConstructor<float>(10.f);
  SharedData<float>value1  = SharedDataConstructor<float>(110.f);
  SharedData<float>result0 = SharedDataConstructor<float>(0.f);
  SharedData<float>result1 = SharedDataConstructor<float>(0.f);
  std::shared_ptr<BaseAddition<float>>add=std::make_shared<BaseAddition<float>>();
  add->Parent::add(value0,value1);
  add->Child::add(result0,result1);
  (*add)();
  std::cout<<"result0: "<<result0->get()<<std::endl;
  std::cout<<"result1: "<<result1->get()<<std::endl;
  return 0;
}
