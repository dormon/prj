#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

void setData(void*ptr){
  std::shared_ptr<int>*sptr=reinterpret_cast<std::shared_ptr<int>*>(ptr);
  **sptr=132;
}

class Base{
  public:
    virtual ~Base(){}
    int a;
};

class ChildA: public Base{
  public:
    float b;
};

class ChildB: public Base{
  public:
    char c;
};

int main(){

  std::shared_ptr<Base>d=std::make_shared<ChildA>();

  std::shared_ptr<ChildA>c0=std::dynamic_pointer_cast<ChildA>(d);
  std::shared_ptr<ChildB>c1=std::dynamic_pointer_cast<ChildB>(d);
  if(c0){
    c0->b=12.2f;
    std::cout<<c0->b<<std::endl;
  }
  if(c1){
    c1->c='a';
    std::cout<<c1->c<<std::endl;
  }

  std::shared_ptr<int>ptr=std::make_shared<int>(100);
  setData(reinterpret_cast<void*>(&ptr));
  std::cout<<*ptr<<std::endl;

  return 0;
}
