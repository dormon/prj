#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

template<typename TYPE>
class Variable{
  public:
    void set(TYPE const& data){
      this->_data = data;
    }
    TYPE get()const{
      return this->_data;
    }
  protected:
    TYPE _data;
};

//sometimes you want differencies for some types
//you can use specialisation
template<> void Variable <std::string> ::set(std::string const& data){
  this->_data = data + data;
}

template<> float Variable <float      > ::get()const{
  return this->_data + 1000.f;
}

int main(){
  Variable<int        >intVariable   ;
  Variable<float      >floatVariable ;
  Variable<std::string>stringVariable;
  Variable<double     >doubleVariable;

  intVariable   .set(12    );
  floatVariable .set(13.3f );
  stringVariable.set("ahoj");
  doubleVariable.set(3232.3);

  std::cout<<intVariable   .get()<<std::endl;
  std::cout<<floatVariable .get()<<std::endl;
  std::cout<<stringVariable.get()<<std::endl;
  std::cout<<doubleVariable.get()<<std::endl;
  return 0;
}
