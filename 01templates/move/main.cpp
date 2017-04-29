#include<iostream>
#include<vector>

class Element{
  public:
    Element(){}
    Element(float const& d):_data(d){
      std::cout<<"constructor"<<std::endl;
    }
    Element(Element const& other):_data(other._data){
      std::cout<<"Copy constructor"<<std::endl;
    }
    ~Element(){}
  protected:
    float _data;

};

std::vector<Element>&& createElements(size_t n){
  std::vector<Element>result;
  result.resize(n);
  return std::move(result);
}


int main(){
  auto const elems = createElements(1);
  return 0;
}

