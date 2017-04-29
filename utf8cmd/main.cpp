#include<iostream>
#include<sstream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

int main(int argc,char*argv[]){
  for(int i=0;i<argc;++i)
    std::cout<<"#"<<argv[i]<<"#"<<" "<<strlen(argv[i])<<std::endl;

  return 0;
}
