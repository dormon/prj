#include<iostream>
#include<sstream>
#include<map>
#include<vector>
#include<typeinfo>

#define ___ std::cerr << __PRETTY_FUNCTION__ << std::endl

class ResourceBase{
  public:
    ResourceBase(){}
    ResourceBase(ResourceBase const&o){init(o.size);}
    void init(size_t const&s){data = new uint8_t[s];size = s;}
    ResourceBase(size_t const&s){init(s);}
    virtual ~ResourceBase(){delete[]data;}
  protected:
    uint8_t*data = nullptr;
    size_t  size = 0      ;
};

template<typename TYPE>
class Resource: public ResourceBase{
  public:
    using type = TYPE;
    Resource():ResourceBase(sizeof(TYPE)){}
    Resource(TYPE const&d):Resource(){new(data)TYPE(d);}
    Resource(Resource<TYPE>const&o):Resource(){new(data)TYPE(static_cast<TYPE>(o));}
    ~Resource(){reinterpret_cast<TYPE*>(data)->~TYPE();}
    template<typename TO,typename std::enable_if<!std::is_same<TO,Resource>::value,unsigned>::type = 0>
      TO const&operator=(TO const&d){*reinterpret_cast<TO*>(data) = d;return d;}
    template<typename TO>
      operator TO&()     {return *reinterpret_cast<TO*>(data);}
    template<typename TO>
      operator TO&()const{return *reinterpret_cast<TO*>(data);}
};

class FunctionBase{
  public:
    using FunctionPointer = void(*)();
    FunctionBase(FunctionPointer const&f):fce(f){}
  protected:
    FunctionPointer fce = nullptr;
};

template<typename T>struct is_resource              : std::false_type{};
template<typename T>struct is_resource<Resource<T>> : std::true_type {};

template<typename RET,typename...ARGS>
class Function:public FunctionBase{
  public:
    using FunctionType = RET(*)(ARGS...);
    Function(FunctionType const&fce):FunctionBase(reinterpret_cast<FunctionPointer>(fce)){}
    template<typename...ARGSS>
      void operator()(ARGSS const&...args){
        storeInputs(args...);
      }
    void storeInputs(){}
    template<typename HEAD,typename...TAIL>
      void storeInputs(HEAD const&h,TAIL const&...tail){
        storeInput(h);
        storeInputs(tail...);
      }
    template<typename T,typename std::enable_if<!is_resource<T>::value,char>::type = 0>
      void storeInput(T const&t){
        inputs.push_back(new Resource<T>(t));
      }
    template<typename T,typename std::enable_if< is_resource<T>::value,char>::type = 0>
      void storeInput(T const&t){
        inputs.push_back(new T(t));
      }
    ~Function(){for(auto const&x:inputs)delete x;}
  protected:
    std::vector<ResourceBase*>inputs;
 };

template<typename RET,typename...ARGS>
Function<RET,ARGS...> func(RET(*f)(ARGS...)){return Function<RET,ARGS...>(f);}

float add(float a,float b){return a+b;}

int main(){
  Resource<float>a(1.f);
  auto b = a;
  //a = 1.f;
  //Resource<float>b(11.f);
  //Resource<float>c(10.f);
  //func(add)(a,2.f);
    

  return 0;
}
