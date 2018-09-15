#include"TypeRegister.h"

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
