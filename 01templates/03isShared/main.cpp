#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

/* you can write your own std::is_*
 * this file implements is_shared
 */

template <typename T> struct is_shared                     : std::false_type{};
template <typename T> struct is_shared<std::shared_ptr<T>> : std::true_type {};

template<typename TYPE>
class Attrib{
  protected:
    TYPE _value;
  public:
    Attrib(TYPE const&value){this->_value = value;}
    Attrib(){}
    //get function will be added only if TYPE is shared_pointer
    template<typename T = TYPE>
    inline typename std::enable_if<is_shared<TYPE>::value,T>::type&get(){return this->_value;}
};



int main(){
  Attrib<std::shared_ptr<int>>*ptr=new Attrib<std::shared_ptr<int>>(std::make_shared<int>(12));
  std::cout<<*ptr->get()<<std::endl;

  Attrib<int>*ptri=new Attrib<int>(13);

  //ptri does not contain get function
  //std::cout<<ptri->get()<<std::endl;

  delete ptr;
  delete ptri;

  if( is_shared<int>::value ){
    std::cout << "je to shared" << std::endl;
  }

  return 0;
}
