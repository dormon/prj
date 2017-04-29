#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

class Base{
  protected:
    virtual void _set(){
      this->_data = 10;
    }
    void _setset(){
      this->_set();
    }
  public:
    int _data;
    virtual ~Base(){}
    Base(){
      this->_setset();
    }
};

class Child: public Base{
  protected:
    virtual void _set(){
      this->_data = 1123;
    }
  public:
    Child():Base(){}
    virtual ~Child(){}
};

int main(){
  Child*a=new Child();
  std::cout<<a->_data<<std::endl;
  delete a;
  return 0;
}





