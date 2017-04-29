#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

#include"int.h"
#include"add.h"
#include"mul.h"
#include"sub.h"
#include"addeqmul.h"
#include"addeqsub.h"
#include"muleqsub.h"

int main(){
  Add*add0=new Add(1,2);
  Add*add1=new Add(2,1);
  Mul*mul0=new Mul(1,3);
  Mul*mul1=new Mul(3,1);
  Sub*sub0=new Sub(5,2);
  Sub*sub1=new Sub(6,3);
  Int*ia=nullptr,*ib=nullptr;


  std::cout<<(*add0==*add1)<<std::endl;
  std::cout<<(*mul0==*mul1)<<std::endl;
  std::cout<<(*sub0==*sub1)<<std::endl;
  std::cout<<(*add0==*sub1)<<std::endl;
  std::cout<<(*add0==*mul1)<<std::endl;
  std::cout<<(*sub0==*mul1)<<std::endl;


  ia=add0;
  ib=sub1;
  std::cout<<(*ia==*ib)<<std::endl;


  delete add0;
  delete add1;
  delete mul0;
  delete mul1;
  delete sub0;
  delete sub1;

  return 0;
}
