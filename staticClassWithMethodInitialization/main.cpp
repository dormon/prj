#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

class Static{
  protected:
    int _num = 0;
  public:
    Static(){
      std::cerr<<"Static::Static()"<<std::endl;
    }
    ~Static(){
      std::cerr<<"Static::~Static()"<<std::endl;
    }
    void setNum(int num){
      std::cerr<<"Static::setNum("<<num<<")"<<std::endl;
      this->_num = num;
    }
    std::string convert(std::string d){
      std::string res="";
      for(int i=0;i<this->_num;++i)
        res+=d;
      return res;
    }
};

std::string get(std::string d){
  static Static a;
  static bool first = true;
  if(first){
    a.setNum(3);//this is called more times 
    first=false;
  }
  return a.convert(d);
}


int main(){
  std::cout<<get("dormon")<<std::endl;
  std::cout<<get("dormon")<<std::endl;
  return 0;
}
