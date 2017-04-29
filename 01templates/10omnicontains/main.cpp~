#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

class Box;

class BindingPointRights{
  public:
    static const unsigned DISABLED    = 0x0;
    static const unsigned READ_ONLY   = 0x1;
    static const unsigned WRITE_ONLY  = 0x2;
    static const unsigned READ_WRITE  = READ_ONLY | WRITE_ONLY;
    static const unsigned BLOCK_READ  = 0x4;
    static const unsigned BLOCK_WRITE = 0x8;
  protected:
    unsigned _readWrite;
  public:
    BindingPointRights(unsigned rights = READ_WRITE){this->_readWrite = rights;}
    inline bool isEnabled  (){return this->_readWrite!=BindingPointRights::DISABLED  ;}
    inline bool canBeRead  (){return this->_readWrite| BindingPointRights::READ_ONLY ;}
    inline bool canBeWriten(){return this->_readWrite| BindingPointRights::WRITE_ONLY;}
    inline void setReadOnly (){this->_readWrite  = BindingPointRights::READ_ONLY ;}
    inline void setWriteOnly(){this->_readWrite  = BindingPointRights::WRITE_ONLY;}
    inline void setReadWrite(){this->_readWrite  = BindingPointRights::READ_WRITE;}
    inline void setDisabled (){this->_readWrite  = BindingPointRights::DISABLED  ;}
    inline void addRead     (){this->_readWrite |= BindingPointRights::READ_ONLY ;}
    inline void addWrite    (){this->_readWrite |= BindingPointRights::WRITE_ONLY;}
};

class BindingPoint: public BindingPointRights{
  protected:
    std::string                  _name  ;
    std::shared_ptr<BindingPoint>_target;
  public:
    BindingPoint(
        std::string                 name   = ""                            ,
        unsigned                    rights = BindingPointRights::READ_WRITE):BindingPointRights(rights){
      this->setName(name);
    }
    inline std::string getName(){return this->_name;}
    inline void        setName(std::string name){this->_name = name;}
    inline std::shared_ptr<BindingPoint>getTarget(){return this->_target;}
    inline void                         setTarget(const std::shared_ptr<BindingPoint>&target){this->_target = target;}
    inline bool operator<(const BindingPoint&other)const{return ((BindingPoint*)this)->getName()<((BindingPoint)other).getName();}
    virtual void write(void*data);
    virtual void*read();
};


class BoxInterface{
  protected:
    std::set<BindingPoint>_bindingPoints;
  public:
    typedef std::set<BindingPoint>::iterator Iterator;
    inline Iterator addBindingPoint   (const BindingPoint&bindingPoint){return this->_bindingPoints.insert(bindingPoint).first;}
    inline void     removeBindingPoint(const BindingPoint&bindingPoint){this->_bindingPoints.erase (bindingPoint);}



};

class Box{
  protected:
    std::shared_ptr<BoxInterface>_interface;
  public:
    
};

template<typename NODE>
class SetOfShared: public std::set<std::shared_ptr<NODE>>{
  public:
    void add(const std::shared_ptr<NODE>&value){
      this->insert(value);
    }
    template<typename...ARGS>
    void add(const std::shared_ptr<NODE>&value,ARGS...args){
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
    BaseData(){};
    BaseData(const TYPE&value){this->set(value);}
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
inline SharedData<TYPE> SharedDataConstructor(ARGS... args){return std::make_shared<BaseData<TYPE>>(args...);}


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
