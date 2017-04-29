#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

/* Class A contains shared pointer to class B
 * Class B contains shared pointer to class A
 * This will create memory leak
 *
 * Solution: use weak pointer in class B instead of shared pointer
 */

class B;//forward declaration

class A{
  public:
    std::shared_ptr<B>_b;
    int _data=32;
    A(int data){
      this->_data = data;
      this->_b = nullptr;
    }
    void setB(std::shared_ptr<B>const&b){
      this->_b = b;
    }

};

class B{
  public:
    std::weak_ptr<A>_a;
    //std::shared_ptr<A>_a;
    float _data=1.2f;
    B(float data,std::weak_ptr<A>const&a){
    //B(float data,std::shared_ptr<A>const&a){
      this->_data = data;
      this->_a = a;
    }
};

int main(){
  std::shared_ptr<A> a=std::make_shared<A>(10101);
  a->setB(std::make_shared<B>(101010.32f,a));
  return 0;
}
