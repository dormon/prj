#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>


void print(){
  std::cout<<"zacatek print()"<<std::endl;
  class Obj{
    protected:
      int*_data = nullptr;
    public:
      Obj(int n){
        this->_data = new int[n];
        std::cout<<"volam constructor"<<std::endl;
      }
      ~Obj(){
        delete[]this->_data;
        std::cout<<"volam destructor"<<std::endl;
      }
  }static o(23);
  std::cout<<"konec print()"<<std::endl;
}

int main(){
  std::cout<<"zacatek main()"<<std::endl;
  print();
  print();
  std::cout<<"konec main()"<<std::endl;
  return 0;
}





