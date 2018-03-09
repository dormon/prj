#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<functional>
#include<stdexcept>
#include<cassert>
#include<sstream>

template<typename R,typename...ARGS>
void call(R(*f)(ARGS...),ARGS const&...args){
  f(args...);
}

void print(){
  std::cout << "print" << std::endl;
}

int main(){
  call(print);

  call(+[](){std::cout << "jojo" << std::endl;});

  return 0;
};
