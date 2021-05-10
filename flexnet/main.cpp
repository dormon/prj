#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include <iomanip>
#include <cmath>

//#define STRINGIFY(S) STRINGIFY2(S)
#define STRINGIFY2(S) #S
#define STRINGIFY(...) #__VA_ARGS__
#define CAPTURE_FILE(...)\
__VA_ARGS__ \
char const*src = STRINGIFY(__VA_ARGS__);

CAPTURE_FILE(//)
int add(int a,int b){
  return a,b;
}


#define ___ std::cerr << __LINE__ << std::endl

std::map<void const*,std::string>names;
std::string getThisName(void const*t){
  auto it = names.find(t);
  if(it!=names.end())return it->second;
  std::stringstream ss;
  ss << names.size();
  names[t] = ss.str();
  return ss.str();
}

#define PF() std::cerr << getThisName(this) << (__PRETTY_FUNCTION__+1) << std::endl

class A{
  public:
    float*d;
    A()         {PF();d=new float;*d=0.f;}
    A(A const&a){std::cerr << getThisName(this) << "::A(A const&"<<getThisName(&a)<<")" << std::endl;d=new float;d[0]=a.d[0];}
    ~A(){PF();delete d;}
    A(A&&a){std::cerr << getThisName(this) << "::A(A&&"<<getThisName(&a)<<")" << std::endl;d=a.d;}
    A&operator=(A const&){std::cerr << getThisName(this) << "::operator==(A const&)" << std::endl;return*this;}

};

A&&operator+(A&&a,A&&b){
  ___;
  A&&r{};
  ___;
  r.d[0] = a.d[0]+b.d[0];
  ___;
  return std::move(r);
}


template<typename>inline std::string getSource();

#define CAPTURE_CLASS(CLASS,IMPL)\
IMPL \
template<>inline std::string getSource<CLASS>(){return STRINGIFY(IMPL);}

class CapturedSource{
  public:
    virtual std::string getSource()const = 0;
};

CAPTURE_CLASS(Floata,//)

class Floata{
  public:
    Floata(float a):data(a){}
    Floata(Floata const&a):data(a.get()){}
    float get()const{return data;}
    void set(float a){data = a;} 
  protected:
    float data;
};

/*(*/)

/*(*/)

class Vars;
using FCE=void(*)(Vars&);

class Vars{
  public:

};

int main()
{
  std::cerr << src << std::endl;
  ___;
  auto a=A();
  ___;
  auto b=A();
  ___;
  auto c=a;//+b;
  ___;
  return 0;
}
