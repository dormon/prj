#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

class Command{
  public:
    Command(int  const& a):data(a){}
    virtual void operator()()const = 0;
    virtual ~Command(){}
    int data;
};

Command*factory(float a){
  class ABomb: public Command{
    public:
      ABomb(float a):Command(10),data(a){}
      virtual void operator()()const override{
        std::cout <<" bum!"<<std::endl;
      }
      float data;
  };
  return new ABomb(a);
}

int main(){
  auto v = factory(10.1f);
  (*v)();
  delete v;
  return 0;
}





