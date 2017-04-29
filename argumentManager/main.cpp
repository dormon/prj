#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

#include"ArgumentManager.h"


int main(int argc,char*argv[]){
  ArgumentManager argm(argc-1,argv+1);
  for(auto x:argm)
    std::cout<<x.first<<": "<<x.second->toStr()<<std::endl;
//  delete new ArgumentManager(argc-1,argv+1);
  return 0;
}
