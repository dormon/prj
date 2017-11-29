#include<iostream>

#include<dsl/FunctionTraits.h>
#include<dsl/TypeSet.h>

class Test{
  public:
    float pow2(float a){
      return a*a;
    }
    float ahoj(float a)const{
      return a+a;
    };
};

float fce(float a){return a*a;}

int main(){
  using List = dsl::TypeList<float,int,char>;
  List a;
  static_cast<void>(a);

  using Set = dsl::TypeSet<float,int,float>;
  Set b;
  static_cast<void>(b);

  using Set2 = dsl::TypeSet<float,int,char>;
  auto const floatPos = Set2::idOf<float>();
  auto const intPos   = Set2::idOf<int  >();
  auto const charPos  = Set2::idOf<char >();
  static_cast<void>(floatPos);
  static_cast<void>(intPos  );
  static_cast<void>(charPos );

  //auto const int16_tPos = Set2::idOf<int16_t>();
  //static_cast<void>(int16_tPos);

  dsl::FunctionTraits      <decltype(fce        )>::returnType a0;
  dsl::FunctionTraits      <decltype(fce        )>::type<0>    a1;
  dsl::FunctionTraits      <decltype(&Test::pow2)>::returnType a2;
  dsl::FunctionTraits      <decltype(&Test::pow2)>::type<0>    a3;

  dsl::MemberFunctionTraits<decltype(&Test::pow2)>::returnType b0;
  dsl::MemberFunctionTraits<decltype(&Test::pow2)>::type<0>    b1;
  dsl::MemberFunctionTraits<decltype(&Test::pow2)>::classType  b2;

  dsl::MemberFunctionTraits<decltype(&Test::ahoj)>::returnType c0;
  dsl::MemberFunctionTraits<decltype(&Test::ahoj)>::type<0>    c1;
  dsl::MemberFunctionTraits<decltype(&Test::ahoj)>::classType  c2;

  static_cast<void>(a0);
  static_cast<void>(a1);
  static_cast<void>(a2);
  static_cast<void>(a3);

  static_cast<void>(b0);
  static_cast<void>(b1);
  static_cast<void>(b2);

  static_cast<void>(c0);
  static_cast<void>(c1);
  static_cast<void>(c2);

  return 0;
}
