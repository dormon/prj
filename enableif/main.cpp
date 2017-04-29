#include<iostream>

template<typename T>
void print( T t){
  std::cout<<t<<std::endl;
}

void p(int&a){
  std::cout<<a<<std::endl;
}

template<unsigned SIZE,typename TYPE>
inline void argsToArray(TYPE data[SIZE],TYPE v){
  data[SIZE-1]=v;
}
template<unsigned SIZE,typename TYPE,typename... Args>
inline void argsToArray(TYPE data[SIZE],TYPE v, Args... args){
  data[SIZE-1-sizeof...(args)]=v;
  argsToArray<SIZE,TYPE>(data,args...);
}

template<unsigned DIM,typename T>
class UniformData{
  protected:
    T _data[DIM];
  public:
    template<typename... ARGS,typename std::enable_if<sizeof...(ARGS) == DIM, unsigned>::type = 0>
    UniformData(ARGS... args){
      argsToArray<DIM,T>(this->_data,args...);
    }
    const T getData(){
      return this->_data;
    }
};

int main(){
  UniformData<3,float>*ud=new UniformData<3,float>(32.f,32.f,32.f);
  int Int=32;
  float Float=23.32;
  std::string String="ahoj";
  //print(Int);
  print(Float);
  int a=23;
  p(a);
  //print(String);
  return 0;
}
