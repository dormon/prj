#include<iostream>
#include<vector>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<sstream>
#include<thread>
#include<mutex>
#include<chrono>
#include<string>
#include<type_traits>
#include<sstream>
#include<typeinfo>
#include<utility>
#include<limits>
#include <glm/glm.hpp>

template<typename T>
class Fop{
  public:
    enum Type{
      CONSTANT      ,
      VARIABLE      ,
      ADDITION      ,
      MULTIPLICATION,
      SUBTRACTION   ,
      DIVISION      ,
    }type;
    Fop(Type const&t):type(t){}
    Fop(Fop<T> const&o):type(o.type){}
    virtual ~Fop(){}
    virtual T operator()(std::vector<T>const&d)const = 0;
};

template<typename T>
class Const: public Fop<T>{
  public:
    T data;
};

template<typename T>
class Var: public Fop<T>{
  public:
    size_t index;
    Var(size_t i = 0):Fop<T>(Fop<T>::VARIABLE),index(i){}
    virtual ~Var(){}
    virtual T operator()(std::vector<T>const&d)const override{
      return d[index];
    }
};

template<typename T>
class Binary: public Fop<T>{
  public:
    Binary(Fop<T> const&t):Fop<T>(t){}
    std::shared_ptr<Fop<T>>op[2] = {nullptr,nullptr};
    virtual ~Binary(){}
};


#define BINARY(NAME,TYPE,OP)                                   \
template<typename T>                                           \
class NAME: public Binary<T>{                                  \
  public:                                                      \
    NAME():Binary<T>(Fop<T>::TYPE){}                           \
    virtual T operator()(std::vector<T>const&d)const override{ \
      return (*Binary<T>::op[0])(d) OP (*Binary<T>::op[1])(d); \
    }                                                          \
}

BINARY(Addition      ,ADDITION      ,+);
BINARY(Multiplication,MULTIPLICATION,*);
BINARY(Subtraction   ,SUBTRACTION   ,-);
BINARY(Divistion     ,DIVISION      ,/);


class Op{
  public:
  enum Type{
    CONST   ,
    VAR     ,
    ADD     ,
    MUL     ,
    SUB     ,
    DIV     ,
    MOD     ,
    LESS    ,
    GREATER ,
    LEQUAL  ,
    GEQUAL  ,
    EQUAL   ,
    NOTEQUAL,
    AND     ,
    OR      ,
    XOR     ,
    BAND    ,
    BOR     ,
    NEG     ,
    NOT     ,
  }type;
  Op(Op const&o){
    type  = o.type;
    a[0]  = o.a[0];
    a[1]  = o.a[1];
    value = o.value;
  }
  Op(std::shared_ptr<Op>const&o1,std::shared_ptr<Op>const&o2,Type const& t){
    a[0] = o1;
    a[1] = o2;
    type = t;
  }
  Op(Op const&o1,Op const&o2,Type const& t):Op(std::make_shared<Op>(o1),std::make_shared<Op>(o2),t){}
  Op(std::shared_ptr<Op>const&o1,uint32_t o2,Type const t){
    a[0] = o1;
    a[1] = std::make_shared<Op>(o2,CONST);
    type = t;
  }
  Op(Op const&o1,uint32_t o2,Type const& t):Op(std::make_shared<Op>(o1),o2,t){}
  Op(uint32_t o1,std::shared_ptr<Op>const&o2,Type const t){
    a[0] = std::make_shared<Op>(o1,CONST);
    a[1] = o2;
    type = t;
  }
  Op(uint32_t o1,Op const&o2,Type const& t):Op(o1,std::make_shared<Op>(o2),t){}
  Op(std::shared_ptr<Op>const&o1,Type const t){
    a[0] = o1;
    type = t;
  }
  Op(Op const&o1,Type const& t):Op(std::make_shared<Op>(o1),t){}
  Op(uint32_t v,Type const t){
    value = v;
    type = t;
  }
  std::shared_ptr<Op>a[2] = {nullptr,nullptr};
  uint32_t value;
  uint32_t operator()(std::vector<uint32_t>const&data){
    switch(type){
      case CONST:
        return value;
      case VAR  : 
        if(value >= data.size())throw std::runtime_error("to large adress");
        return data[value];
      case ADD     :return  (*a[0])(data) +  (*a[1])(data);
      case MUL     :return  (*a[0])(data) *  (*a[1])(data);
      case SUB     :return  (*a[0])(data) -  (*a[1])(data);
      case DIV     :return  (*a[0])(data) /  (*a[1])(data);
      case MOD     :return  (*a[0])(data) %  (*a[1])(data);
      case LESS    :return  (*a[0])(data) <  (*a[1])(data);
      case GREATER :return  (*a[0])(data) >  (*a[1])(data);
      case LEQUAL  :return  (*a[0])(data) <= (*a[1])(data);
      case GEQUAL  :return  (*a[0])(data) >= (*a[1])(data);
      case EQUAL   :return  (*a[0])(data) == (*a[1])(data);
      case NOTEQUAL:return  (*a[0])(data) != (*a[1])(data);
      case AND     :return  (*a[0])(data) && (*a[1])(data);
      case OR      :return  (*a[0])(data) || (*a[1])(data);
      case XOR     :return  (*a[0])(data) ^  (*a[1])(data);
      case BAND    :return  (*a[0])(data) &  (*a[1])(data);
      case BOR     :return  (*a[0])(data) |  (*a[1])(data);
      case NEG     :return ~(*a[0])(data)                 ;
      case NOT     :return !(*a[0])(data)                 ;
    }
    throw std::runtime_error("unrecognized op");
  }
  template<typename...ARGS>
    uint32_t operator()(ARGS const&...args){
      std::vector<uint32_t>v = {static_cast<uint32_t>(args)...};
      return operator()(v);
    }
};

Op operator+ (Op       const&a,Op       const&b){return Op(a,b,Op::ADD     );}
Op operator* (Op       const&a,Op       const&b){return Op(a,b,Op::MUL     );}
Op operator- (Op       const&a,Op       const&b){return Op(a,b,Op::SUB     );}
Op operator/ (Op       const&a,Op       const&b){return Op(a,b,Op::DIV     );}
Op operator% (Op       const&a,Op       const&b){return Op(a,b,Op::MOD     );}
Op operator< (Op       const&a,Op       const&b){return Op(a,b,Op::LESS    );}
Op operator> (Op       const&a,Op       const&b){return Op(a,b,Op::GREATER );}
Op operator<=(Op       const&a,Op       const&b){return Op(a,b,Op::LEQUAL  );}
Op operator>=(Op       const&a,Op       const&b){return Op(a,b,Op::GREATER );}
Op operator==(Op       const&a,Op       const&b){return Op(a,b,Op::EQUAL   );}
Op operator!=(Op       const&a,Op       const&b){return Op(a,b,Op::NOTEQUAL);}
Op operator&&(Op       const&a,Op       const&b){return Op(a,b,Op::AND     );}
Op operator||(Op       const&a,Op       const&b){return Op(a,b,Op::OR      );}
Op operator^ (Op       const&a,Op       const&b){return Op(a,b,Op::XOR     );}
Op operator& (Op       const&a,Op       const&b){return Op(a,b,Op::BAND    );}
Op operator| (Op       const&a,Op       const&b){return Op(a,b,Op::BOR     );}
Op operator~ (Op       const&a                 ){return Op(a  ,Op::NEG     );}
Op operator! (Op       const&a                 ){return Op(a  ,Op::NOT     );}
Op operator+ (Op       const&a,uint32_t       b){return Op(a,b,Op::ADD     );}
Op operator* (Op       const&a,uint32_t       b){return Op(a,b,Op::MUL     );}
Op operator- (Op       const&a,uint32_t       b){return Op(a,b,Op::SUB     );}
Op operator/ (Op       const&a,uint32_t       b){return Op(a,b,Op::DIV     );}
Op operator% (Op       const&a,uint32_t       b){return Op(a,b,Op::MOD     );}
Op operator< (Op       const&a,uint32_t       b){return Op(a,b,Op::LESS    );}
Op operator> (Op       const&a,uint32_t       b){return Op(a,b,Op::GREATER );}
Op operator<=(Op       const&a,uint32_t       b){return Op(a,b,Op::LEQUAL  );}
Op operator>=(Op       const&a,uint32_t       b){return Op(a,b,Op::GEQUAL  );}
Op operator==(Op       const&a,uint32_t       b){return Op(a,b,Op::EQUAL   );}
Op operator!=(Op       const&a,uint32_t       b){return Op(a,b,Op::NOTEQUAL);}
Op operator&&(Op       const&a,uint32_t       b){return Op(a,b,Op::AND     );}
Op operator||(Op       const&a,uint32_t       b){return Op(a,b,Op::OR      );}
Op operator^ (Op       const&a,uint32_t       b){return Op(a,b,Op::XOR     );}
Op operator& (Op       const&a,uint32_t       b){return Op(a,b,Op::BAND    );}
Op operator| (Op       const&a,uint32_t       b){return Op(a,b,Op::BOR     );}
Op operator+ (uint32_t       a,Op       const&b){return Op(a,b,Op::ADD     );}
Op operator* (uint32_t       a,Op       const&b){return Op(a,b,Op::MUL     );}
Op operator- (uint32_t       a,Op       const&b){return Op(a,b,Op::SUB     );}
Op operator/ (uint32_t       a,Op       const&b){return Op(a,b,Op::DIV     );}
Op operator% (uint32_t       a,Op       const&b){return Op(a,b,Op::MOD     );}
Op operator< (uint32_t       a,Op       const&b){return Op(a,b,Op::LESS    );}
Op operator> (uint32_t       a,Op       const&b){return Op(a,b,Op::GREATER );}
Op operator<=(uint32_t       a,Op       const&b){return Op(a,b,Op::LEQUAL  );}
Op operator>=(uint32_t       a,Op       const&b){return Op(a,b,Op::GEQUAL  );}
Op operator==(uint32_t       a,Op       const&b){return Op(a,b,Op::EQUAL   );}
Op operator!=(uint32_t       a,Op       const&b){return Op(a,b,Op::NOTEQUAL);}
Op operator&&(uint32_t       a,Op       const&b){return Op(a,b,Op::AND     );}
Op operator||(uint32_t       a,Op       const&b){return Op(a,b,Op::OR      );}
Op operator^ (uint32_t       a,Op       const&b){return Op(a,b,Op::XOR     );}
Op operator& (uint32_t       a,Op       const&b){return Op(a,b,Op::BAND    );}
Op operator| (uint32_t       a,Op       const&b){return Op(a,b,Op::BOR     );}
Op var       (uint32_t       v                 ){return Op(v,  Op::VAR     );}

int main(){
  auto f = var(0)+var(1);
  std::cerr << f(1,2) << std::endl;
  return 0;
}
