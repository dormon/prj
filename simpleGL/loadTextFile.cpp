#include"loadTextFile.h"
#include<fstream>

std::string loadTextFile(std::string file){
  std::ifstream f(file.c_str());
  if(!f.good()){
    std::cerr<<"ERROR: cannot open: "<<file<<std::endl;
    return "";
  }
  f.seekg(0,std::ios::end);
  std::size_t size=f.tellg();
  f.seekg(0,std::ios::beg);
  char*buffer=new char[size+1];
  f.read(buffer,size);
  buffer[size]='\0';
  std::string result(buffer);
  delete[]buffer;
  f.close();
  return result;
}


