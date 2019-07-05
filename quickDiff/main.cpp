#include<iostream>
#include <sstream>
#include<vector>
#include<memory>
#include <cstring>
#include <functional>
#include <cassert>

class Op{
  public:
    enum Type{
      ADD  ,
      SUB  ,
      MUL  ,
      DIV  ,
      POW  ,
      CONST,
      VAR  ,
      USUB ,
      SIN  ,
      COS  ,
      LOG  ,
      EXP  ,
    }type;
    Op(Op const&aa,Op const&bb,Type t):type(t),a(std::make_shared<Op>(aa)),b(std::make_shared<Op>(bb)){}
    Op(Op const&aa,Type t):type(t),a(std::make_shared<Op>(aa)){}
    Op(size_t v):type(VAR),var(v){}
    Op(float cc):type(CONST),c(cc){}
    float c;
    size_t var;
    std::shared_ptr<Op>a;
    std::shared_ptr<Op>b;
    bool brackets(Type const&parent,bool left)const{
      auto const right = !left;
      if((type == ADD || type == SUB) && (parent == MUL || parent == DIV) )return true;
      if(left == false && parent == SUB && (type == ADD || type == SUB))return true;
      if(right && parent == DIV && (type == MUL || type == DIV))return true;
      if(parent == USUB && (type == ADD || type == SUB || type == USUB))return true;
      return false;
    }
    std::string str(Type parent = CONST,bool left = true)const{
      auto const bin = [&](std::stringstream &ss,std::string const&op){
        assert(a);
        assert(b);
        if(brackets(parent,left))ss<<"(";
        ss << a->str(type,true) << op << b->str(type,false);
        if(brackets(parent,left))ss<<")";
      };
      auto const una = [&](std::stringstream &ss,std::string const&n ){
        assert(a);
        ss << n << "(" << a->str() << ")";
      };
      auto const usub = [&](std::stringstream &ss){
        assert(a);
        if(brackets(parent,left))ss << "(";
        ss << "-";
        ss << a->str(type);
        if(brackets(parent,left))ss << ")";
      };
      std::stringstream ss;
      switch(type){
        case ADD  :bin(ss,"+");break;
        case SUB  :bin(ss,"-");break;
        case MUL  :bin(ss,"*");break;
        case DIV  :bin(ss,"/");break;
        case POW  :bin(ss,"^");break;
        case CONST:ss << c;break;
        case VAR  :ss << "x[" << var << "]";break;
        case USUB :usub(ss);break;
        case SIN  :una(ss,"sin");break;
        case COS  :una(ss,"cos");break;
        case LOG  :una(ss,"log");break;
        case EXP  :una(ss,"exp");break;
      }
      return ss.str();
    }
    bool operator<(Op const&o)const{
      if(type == CONST && o.type == VAR)return true;
      return false;
    }
};

std::ostream& operator<<(std::ostream& os,Op const&o){
  os << o.str();
  return os;
}


Op var(size_t v){
  return Op(v);
}

Op cst(float c){
  return Op(c);
}

#define BINARY(OP,TYPE)\
Op operator OP (Op const&a,Op const&b){\
  return Op(a,b,Op::TYPE);\
}\
Op operator OP (Op const&a,float b){\
  return Op(a,Op(b),Op::TYPE);\
}\
Op operator OP (float a,Op const&b){\
  return Op(Op(a),b,Op::TYPE);\
}

BINARY(+,ADD);
BINARY(-,SUB);
BINARY(*,MUL);
BINARY(/,DIV);
BINARY(^,POW);

#define UNARY(FCE,TYPE)\
Op FCE(Op const&a){\
  return Op(a,Op::TYPE);\
}

UNARY(sin,SIN);
UNARY(cos,COS);
UNARY(log,LOG);
UNARY(exp,EXP);

Op operator-(Op const&a){
  return Op(a,Op::USUB);
}

Op operator+(Op const&a){
  return a;
}

Op sort(Op const&a){
  switch(a.type){
    case Op::ADD  ://TODO
    case Op::SUB  :return a;
    case Op::MUL  ://TODO
    case Op::DIV  :return a;
    case Op::POW  :return a;
    case Op::CONST:return a;
    case Op::VAR  :return a;
    case Op::USUB :return a;
    case Op::SIN  :return a;
    case Op::COS  :return a;
    case Op::LOG  :return a;
    case Op::EXP  :return a;
  }
  return a;
}

Op applyUSUB(Op const&a){
  if(a.type == Op::USUB)return *a.a;
  if(a.type == Op::ADD)return applyUSUB(*a.a)+applyUSUB(*a.b);
  if(a.type == Op::SUB)return applyUSUB(*a.a)+*a.b;
  if(a.type == Op::CONST)return Op(-a.c);
  return Op(a,Op::USUB);
}

Op optimize(Op const&a){
  return sort(a);
}

void test(Op const&o,std::string const&t){
  if(o.str() != t)
    std::cerr << o.str() << " != " << t << std::endl;
}

int main(){
  test(var(0)                     , "x[0]"            );
  test(var(0)-var(1)-var(2)       , "x[0]-x[1]-x[2]"  );
  test(var(0)-(var(1)-var(2))     , "x[0]-(x[1]-x[2])");
  test((var(2) + var(3)) * var(4) , "(x[2]+x[3])*x[4]");
  test(-(-var(0))                 , "-(-x[0])"        );
  test(var(0)/(var(1)/var(2))     , "x[0]/(x[1]/x[2])");
  //test(-var(0) * -var(1)     , "");
  return 0;
}


