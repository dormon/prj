#include<iostream>
#include<vector>
#include<memory>
#include <cstring>
#include <functional>
#include <sstream>

template<typename T>
std::string toStr(T const&t){
  std::stringstream ss;
  ss << t;
  return ss.str();
}

std::vector<std::string>split(std::string txt,std::string const&splitter){
  std::vector<std::string>result;
  for(;;){
    auto where = txt.find(splitter);
    if(where == std::string::npos)break;
    result.push_back(txt.substr(0,where));
    txt = txt.substr(where+splitter.size());
  }
  result.push_back(txt);
  return result;
}

template<typename... ARGS>
std::string fillValues(std::string const&a,ARGS const&...args){
  std::vector<std::string>svalues = {toStr(args)...};
  auto orig = split(a,"%%");
  std::stringstream ss;
  for(size_t i=0;i<orig.size();++i){
    ss << orig[i];
    if(i < svalues.size())ss << svalues[i];
  }
  return ss.str();
}

int main(){
  //for(auto const&x:split("a%%b","%%"))
  //  std::cerr << "#" << x << "#" << std::endl;
  //for(auto const&x:split("%%x","%%"))
  //  std::cerr << "#" << x << "#" << std::endl;
  //for(auto const&x:split("y%%","%%"))
  //  std::cerr << "#" << x << "#" << std::endl;
  //for(auto const&x:split("%%asd%%asd%%sds%%","%%"))
  //  std::cerr << "#" << x << "#" << std::endl;

  std::string va = "dormon";
  std::string static const src = fillValues(R".(
#version 450

layout(local_size_x=%%,local_size_y=%%)in;

  ).",16,16);
  std::cerr << src << std::endl;
}


