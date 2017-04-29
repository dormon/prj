#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

class Acc;

class Man{
  public:
    float asd;
    template<typename... ARGS>
      Acc*newAcc(ARGS... args);
    void asdasdsa(){
      std::cout<<"SDSDSDSDS"<<std::endl;
    }
};

class Acc{
  public:
    int data[10];
    void set(){}
    template<typename... ARGS>
      void set(int a,ARGS... args);
    void print();
    void asdasd(){
      std::cout<<"ASDASD"<<std::endl;
    }
};


template<typename... ARGS>
Acc*Man::newAcc(ARGS... args){
  asd=123123.;
  Acc*ac=new Acc();
  //ac->set(args...);
  return ac;
}

template<typename... ARGS>
void Acc::set(int a,ARGS... args){
  this->data[10-sizeof...(args)]=a;
  set(args...);
}

void Acc::print(){
  for(int i=0;i<10;++i)
    std::cout<<i<<" ";
  std::cout<<std::endl;
}


int main(){

  Man*m=new Man();
  Acc*ac=m->newAcc(1,2,3,4,5);
  delete ac;
  delete m;
  return 0;
}
