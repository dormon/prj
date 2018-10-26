#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<functional>

using FCE = int(*)(int,int);

int add(int a,int b){
  return a+b;
}

class Table{
  public:
    std::function<int(int,int)> fce = [&](int a,int b){
      std::cerr << "start" << std::endl;
      fce = add;
      return add(a,b);
    };
};

int main(){

  Table a;
  std::cerr << a.fce(10,23) << std::endl;
  std::cerr << a.fce(132,23) << std::endl;


  return 0;
}
