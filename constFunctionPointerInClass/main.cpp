#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

typedef void(*FCE)();

class FceTable{
  public:
    FCE a;
    FCE b;
};

class Base{
  protected:
    std::shared_ptr<FceTable>_table = nullptr;
    inline std::shared_ptr<FceTable>_getTable()const{return this->_table;}
  public:
    Base(std::shared_ptr<FceTable>table){this->_table = table;}
    void glA()const{this->_getTable()->a();}
    void glB()const{this->_getTable()->b();}
};

class Child: protected Base{
  public:
    Child(std::shared_ptr<FceTable>table):Base(table){}
    void const_call()const{this->glA();}
};


int main(){
  return 0;
}


