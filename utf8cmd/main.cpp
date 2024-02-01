#include<iostream>
#include<sstream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

int main(int argc,char*argv[]){
  for(int i=0;i<argc;++i){
    auto str = std::string(argv[i]);
    std::cout<<"#"<<str<<"#"<<" str.length() = "<<str.length()<<std::endl;
  }

  return 0;
}
