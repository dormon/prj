#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

//float    Float
//uint32_t Uint32



#define STRINGIFY(S) STRINGIFY2(S)
#define STRINGIFY2(S) #S

template<typename>inline std::string getSource();

#define CAPTURE_CLASS(CLASS,IMPL)\
IMPL \
template<>inline std::string getSource<CLASS>(){return STRINGIFY(IMPL);}



CAPTURE_CLASS(Float,//)

class Float{
  public:
    Float(float a):data(a){}
    Float(Float const&a):data(a.get()){}
    float get()const{return data;}
    void set(float a){data = a;} 
  protected:
    float data;
};

/*(*/)




int main(){
  Float aVal(13);
  std::cout << aVal.get() << std::endl;
  std::cout << getSource<Float>() << std::endl;
  return 0;
}





