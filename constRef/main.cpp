#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

class Obj{
  public:
    std::string data;
    Obj(Obj &&other):data(std::move(other.data)) {
 //     this->data=std::move(other.data);
    }
    Obj(Obj const&other){
      std::cerr<<"Obj::Obj(Obj const&other)"<<std::endl;
      this->data = other.data;
    }
    Obj(std::string a){
      std::cerr<<"Obj::Obj(int a)"<<std::endl;
      this->data = a;
    }
    Obj(){
      std::cerr<<"Obj::Obj()"<<std::endl;
    }
    ~Obj(){
      std::cerr<<"Obj::~Obj()"<<std::endl;
    }



};

std::string asd(Obj const&a){
  return a.data;
}

std::string asd2(Obj &&a){
  return a.data;
}

int main(int argc,char*argv[]){
  std::cout<<asd(Obj("asd"))<<std::endl;
  Obj a("3232");
  std::cout<<asd2(std::move(a))<<std::endl;
  std::cout<<a.data<<std::endl;

  return 0;
}
