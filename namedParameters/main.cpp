#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

int func(int a,int b){
  return a+b;
}

class FuncAB{
  public:
    FuncAB(int const&a,int const&b):aa(a),bb(b){}
    int operator()(){
      return func(aa,bb);
    }
  protected:
    int const&aa;
    int const&bb;
};

class FuncA{
  public:
    FuncA(int const&a):aa(a){}
    FuncAB setB(int const&b){
      return FuncAB{aa,b};
    }
    int operator()(){
      return func(aa,0);
    }
  protected:
    int const&aa;
};

class FuncB{
  public:
    FuncB(int const&b):bb(b){}
    FuncAB setA(int const&a){
      return FuncAB{a,bb};
    }
    int operator()(){
      return func(0,bb);
    }
  protected:
    int const&bb;
};

class Func{
  public:
    FuncA setA(int const&a){
      return FuncA{a};
    }
    FuncB setB(int const&b){
      return FuncB{b};
    }
    int operator()(){
      return func(0,0);
    }
  protected:
};

template<typename T,typename...ARGS>
using FCE = void(*)(T&,ARGS const&...);

template<typename T,typename...ARGS>
FCE<T,ARGS...> getCall(T(*f)(ARGS...)){
  return [&](T&r,ARGS const&...args){r=f(args...);};
}


int main(){
  auto f = getCall(func);
  int r=0,a=12,b=32;
  f(r,a,b);
  std::cerr << Func{}                    () << std::endl;
  std::cerr << Func{}.setA(23 )          () << std::endl;
  std::cerr << Func{}.setB(200)          () << std::endl;
  std::cerr << Func{}.setA(23 ).setB(200)() << std::endl;
  std::cerr << Func{}.setB(200).setA(23 )() << std::endl;

  return 0;
};

/*
template<typename T>
class Ref{
  public:
    Ref(T&v):d(v){}
    T&get(){return d;}
  protected:
    T&d;
};

class FuncA;

class FuncAB{
  public:
    FuncAB(FuncA&av,int&v):a(av),b(v){}
    int operator()();
    int&b;
    FuncA&a;
};

class FuncA{
  public:
    FuncA(int&v):a(v){}
    FuncAB setB(int&b){
      return FuncAB{*this,b};
    }
    int&get(){return a;}
    int&a;
};

int FuncAB::operator()(){
  return a.a+b;
}

class Func{
  public:
    Func(int a = 0,int b = 0):aa(a),bb(b){}
    Func& setA(int a){
      aa = a;
      return *this;
    }
    Func& setB(int b){
      bb = b;
      return *this;
    }
    int operator()()const{
      return aa+bb;
    }
  protected:
    int aa;
    int bb;
};


int main(){
  Func().setA(10).setB(9)();
  return 0;
}

*/
