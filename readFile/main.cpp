#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<fstream>
#include<sstream>

std::string readTextFile(std::string const& file){
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

int main(){
  std::cout<<readTextFile("main.cpp")<<std::endl;
  return 0;
}
