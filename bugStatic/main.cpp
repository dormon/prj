#include<iostream>

class Inner;

class MyStr{
  public:
    int*data;
    MyStr(){
      std::cerr<<"MyStr::MyStr()"<<std::endl;
      this->data = new int;
    }
    ~MyStr(){
      std::cerr<<"MyStr::~MyStr()"<<std::endl;
      delete this->data;
      this->data = nullptr;
    }
    int&operator=(int const&r){
      *this->data=r;
      return *this->data;
    }
    int get()const{return *this->data;}
};

struct ParData{
  MyStr str2;
};

class Inner{
  public:
    ParData* _data = nullptr;
  public:
    Inner(ParData*d){
      std::cerr<<"Inner::Inner()"<<std::endl;
      this->_data = d;
    }
    ~Inner(){
      std::cerr<<"Inner::~Inner()"<<std::endl;
    }
    void convert(){
      std::cerr<<"data: "<<this->_data->str2.get()<<std::endl;
    }
};

class Token{
  public:
    ParData _parData;
    Token(){
      std::cerr<<"Token::Token()"<<std::endl;
      this->parse();
    }
    ~Token(){
      std::cerr<<"Token::~Token()"<<std::endl;
    }
    void parse(){
      this->_parData.str2=10;
      static Inner a(&this->_parData);
      a.convert();
    }
};

int main(){
  std::string asd="garbage";
  Token();
  Token();
  return 0;
}
