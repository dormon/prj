#include<iostream>

class Inner;

struct ParData{
  std::string str;
};

class Inner{
  public:
    ParData* _data = nullptr;
  public:
    Inner(ParData*d){
      this->_data = d;
    }
    ~Inner(){
    }
    void convert(){
      std::cerr<<"data: "<<this->_data->str<<std::endl;
    }
};

class Token{
  public:
    std::string data;
    ParData _parData;
    Token(){
      this->data = this->parse();
    }
    ~Token(){}
    std::string parse(){
      this->_parData.str="a";
      static Inner a(&this->_parData);
      a.convert();
      return "neco";
    }
};

int main(){
  std::string asd="garbage";
  Token();
  Token();
  return 0;
}
