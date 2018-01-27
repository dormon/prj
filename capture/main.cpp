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


std::string static const nn = STRINGIFY(

#define AHOJ 10
int main (){
  return AHOJ;
}


);

int main(){
  //Float aVal(13);
  //std::cout << aVal.get() << std::endl;
  //std::cout << getSource<Float>() << std::endl;
  std::cout << nn << std::endl;
  return 0;
}





