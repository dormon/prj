#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

class Command{
  public:
    virtual void foo(int a)const{
      std::cout << a << std::endl;
    }
};

class Child: public Command{
  public:
    virtual void foo(int a)const override{
      std::cout << a*a << std::endl;
    }
};

int main(){
  Command* v = new Child();
  v->foo(10);
  delete v;
  return 0;
}





