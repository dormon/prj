#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

/* Consider these two classes -> Integer and Float
 * They do the same job but for different types
 */

class IntegerVariable{
  public:
    void set(int const& data){
      this->_data = data;
    }
    int get()const{
      return this->_data;
    }
  protected:
    int _data = 0;
};

class FloatVariable{
  public:
    void set(float const& data){
      this->_data = data;
    }
    float get()const{
      return this->_data;
    }
  protected:
    float _data = 0.f;
};

/* you can create one generic templated class
 * implementation has to be in header file
 */
template<typename TYPE>
class Variable{
  public:
    void set(TYPE const& data){
      this->_data = data;
    }
    TYPE get()const;//you can implement it later
  protected:
    TYPE _data;
};

//sometimes it is necessary to implement methods later due to cross class dependencies
template<typename TYPE>TYPE Variable<TYPE>::get()const{
  return this->_data;
}

int main(){
  Variable<int        >intVariable   ;
  Variable<float      >floatVariable ;
  Variable<std::string>stringVariable;

  intVariable   .set(12    );
  floatVariable .set(13.3f );
  stringVariable.set("ahoj");

  std::cout<<intVariable   .get()<<std::endl;
  std::cout<<floatVariable .get()<<std::endl;
  std::cout<<stringVariable.get()<<std::endl;
  return 0;
}
