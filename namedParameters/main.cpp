#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

int func(int a,int b){
  return a+b;
}

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
