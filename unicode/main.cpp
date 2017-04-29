#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
//#include<geGL/geGL.h>
#include<GL/glew.h>
#include<cstdlib>
#include<cstring>
#include"attrib.h"

const char* operator "" _upper (const char*vec,size_t){
  unsigned len=std::strlen(vec);
  char*temp=new char[len+1];
  for(unsigned i=0;i<len;++i)
    temp[i]=vec[i]+'A'-'a';
  temp[len]='\0';
  return temp;
}

int main(){

  const char* asd="ada"_upper;
  std::cout<<asd<<std::endl;
  delete[]asd;

  return 0;
}





