#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

/* sometimes you want to disable members of class without exposing error
 */
template<typename TYPE>
class Variable{
  protected:
    TYPE _data;
  public:
    void set(TYPE data){
      this->_data = data;
    }
    template<typename T = TYPE>
    typename std::enable_if<std::is_integral<T>::value,T>::type get()const{
      return this->_data;
    }
};

/* std::enable_if<CONDITION,TYPE>::type
 * it "returns" TYPE if CONDITION is true
 * if CONDITION is not true it does not have ::type member so it fails
 *
 * std::is_integral<TYPE>::value
 * it "return" true if TYPE is integral
 *
 * please note that typename before std::enable_if is mandatory
 */

int main(){
  Variable<int>intVariable;
  intVariable.set(12);

  Variable<float>floatVariable;
  floatVariable.set(13.3f);

  //this will work
  std::cout<<intVariable.get()<<std::endl;
  //this wont work
  //std::cout<<floatVariable.get()<<std::endl;
  return 0;
}
