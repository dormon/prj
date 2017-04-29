#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

class Fce{
  protected:
    virtual inline unsigned const& _getData()const{
      return this->_data;
    }
    virtual inline unsigned & _getData(){
      return this->_data;
    }
  public:
    unsigned _data = 0;
    Fce(){}
    virtual ~Fce(){}
    void setData(unsigned d){
      this->_getData()=d;
    }
    unsigned getData()const{
      return this->_getData();
    }
};

class Var: public Fce{
  protected:
    virtual inline unsigned const& _getData()const{
      return this->_mojData;
    }
    virtual inline unsigned & _getData(){
      return this->_mojData;
    }
  public:
    unsigned _mojData = 0;

};

int main(){
  Fce*a=new Fce();
  a->setData(23);
  std::cout<<a->_data<<std::endl;
  delete a;

  Var*b=new Var();
  b->setData(101);
  std::cout<<"b->_data: "<<b->_data<<std::endl;
  std::cout<<"b->_mojData: "<<b->_mojData<<std::endl;
  delete b;
  return 0;
}





