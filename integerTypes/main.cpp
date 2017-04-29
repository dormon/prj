#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>


template<typename TYPE>
class Function{
  public:
    typename std::enable_if<std::is_integral<TYPE>::value,TYPE>::type asd;
};

int main(){
  Function<int>a;
  a.asd=321;
  std::cout<<a.asd<<std::endl;
  Function<unsigned>b;
  Function<unsigned short>c;
  Function<unsigned char>d;
  b.asd=c.asd=d.asd=a.asd;
  b.asd*=2;
  if(std::is_integral<uint32_t&>::value)std::cerr<<"uint32_t&"<<" je integral type"<<std::endl;
  return 0;
}
