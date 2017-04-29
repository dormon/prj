#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

template <class T> struct is_shared                      : std::false_type {};
template <class T> struct is_shared<std::shared_ptr<T>>  : std::true_type {};

template<typename T>
bool isShared(std::shared_ptr<T>){
  return true;
}

template<typename T>
bool isShared(T){
  return false;
}



template<typename TYPE>
class Attrib{
  protected:
    TYPE _value;
  public:
    Attrib(TYPE const&value){this->_value = value;}
    Attrib(){}
    template<typename T = TYPE>
    inline typename std::enable_if<is_shared<TYPE>::value,T>::type&get(){return this->_value;}
};



int main(){
  Attrib<std::shared_ptr<int>>*ptr=new Attrib<std::shared_ptr<int>>(std::make_shared<int>(12));
  std::cout<<*ptr->get()<<std::endl;

  Attrib<int>*ptri=new Attrib<int>(13);

  delete ptr;
  delete ptri;

  std::cout<<isShared(12)<<std::endl;
  std::cout<<isShared(std::make_shared<int>(13))<<std::endl;
  //std::cout<<ptri->get()<<std::endl;//NEFUNGUJE


  return 0;
}
