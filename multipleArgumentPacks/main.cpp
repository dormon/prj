#include<iostream>
#include<memory>

template<typename TO,typename FROM>
TO convert(FROM a);

template<typename...TO,typename...FROM>
void call(void(&fce)(TO...),FROM...args0){
  fce(convert<TO,FROM>(args0)...);
}

template<>
int32_t convert(int32_t a){
  return a;
}

template<>
int32_t convert(std::shared_ptr<int32_t> a){
  return *a;
}

void printInts(int32_t a,int32_t b){
  std::cout<<a<<std::endl;
  std::cout<<b<<std::endl;
}

int main(){
  int32_t a=10;
  auto pa = std::make_shared<int32_t>(1212);
  call(printInts,a,pa);
  return 0;
}

