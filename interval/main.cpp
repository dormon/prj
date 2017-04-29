#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<butil/integer/interval.h>


int main(){
  using t0 = butil::integer::interval<uint32_t,0 ,10 >;
  using t1 = butil::integer::interval<uint32_t,32,100>;
  using t2 = butil::integer::interval_join<t0,t1>;
  t2 asd;
  std::cout<<asd.min<<std::endl;
  std::cout<<asd.max<<std::endl;

  return 0;
}
