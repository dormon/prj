#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

class Command{
  public:
    virtual void operator()(){}
};

class Fce: public Command{
  protected:
    virtual void _do(){
      std::cout<<"volam Fce::_do()"<<std::endl;
    }
  public:
    void operator()(){
      std::cout<<"begin"<<std::endl;
      this->_do();
      std::cout<<"end"  <<std::endl;
    }
};

class Sin: public Fce{
  protected:
    virtual void _do(){
      std::cout<<"volam Sin::_do()"<<std::endl;
    }
  public:

};

int main(){
  Sin s;
  s();
  return 0;
}





