#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

/* sometimes you want to create templated class that only accept certain kinds of template types
 */
template<typename TYPE>
class Variable{
  public:
    void set(typename std::enable_if<std::is_integral<TYPE>::value,TYPE>::type const& data){
      this->_data = data;
    }
    typename std::enable_if<std::is_integral<TYPE>::value,TYPE>::type get()const{
      return this->_data;
    }
  protected:
    TYPE _data;
};

template<typename TYPE,typename std::enable_if<std::is_integral<TYPE>::value,int32_t>::type = 0>
TYPE leftShift(TYPE const&v){
  return v<<1;
}

/* std::enable_if<CONDITION,TYPE>::type
 * it "returns" TYPE if CONDITION is true
 * if CONDITION is not true it does not have ::type member so it fails
 *
 * std::is_integral<TYPE>::value
 * it "return" true if TYPE is integral
 *
 * please note that typename before std::enable_if is mandatory
 */

template<size_t S = 3>
void print(){
  std::cout << S << std::endl;
}

int main(){
  //this will work
  Variable<int>intVariable;
  intVariable.set(12);
  std::cout<<intVariable.get()<<std::endl;

  std::cout<<leftShift(3)<<std::endl;

  print<10>();
  print();

  //this wont work
  //Variable<float>floatVariable;
  //floatVariable.set(13.3f);
  //std::cout<<floatVariable.get()<<std::endl;
  //std::cout<<leftShift(3.f)<<std::endl;
  return 0;
}
