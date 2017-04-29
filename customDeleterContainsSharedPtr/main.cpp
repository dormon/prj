#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

class Accessor{
  public:
    unsigned _id = 0u;
    std::shared_ptr<char>_data = nullptr;
    Accessor(unsigned id){
      this->_id = id;
      this->_data = std::shared_ptr<char>(new char[10],[&](char*ptr){
          std::cout<<"deleter id: "<<this->_id<<std::endl;//this could not exists because its can be deleted by now
          delete[]ptr;});
    }
    Accessor(Accessor const&ac){
      this->_id   = ac._id  ;
      this->_data = ac._data;
    }
    ~Accessor(){
      std::cout<<"~Accessor"<<std::endl;
    }
};

class Bccessor{
  public:
    std::shared_ptr<unsigned> _id = 0u;
    std::shared_ptr<char>_data = nullptr;
    Bccessor(unsigned id){
      this->_id = std::make_shared<unsigned>(id);
      std::shared_ptr<unsigned>iddqd=this->_id;
      this->_data = std::shared_ptr<char>(new char[10],[iddqd](char*ptr){
          std::cout<<"deleter id: "<<*iddqd<<std::endl;
          delete[]ptr;});
    }
    Bccessor(Bccessor const&ac){
      this->_id   = ac._id  ;
      this->_data = ac._data;
    }
    ~Bccessor(){
      std::cout<<"~Bccessor"<<std::endl;
    }
};

class Cccessor{
  public:
    unsigned _id = 0u;
    std::shared_ptr<char>_data = nullptr;
    Cccessor(unsigned id){
      this->_id = id;
      unsigned iddqd=this->_id;
      this->_data = std::shared_ptr<char>(new char[10],[iddqd](char*ptr){
          std::cout<<"deleter id: "<<iddqd<<std::endl;
          delete[]ptr;});
    }
    Cccessor(Cccessor const&ac){
      this->_id   = ac._id  ;
      this->_data = ac._data;
    }
    ~Cccessor(){
      std::cout<<"~Cccessor"<<std::endl;
    }
};


int main(){
  /*
  Accessor*a=new Accessor(100);
  Accessor*b=new Accessor(*a);
  delete a;
  delete b;
  */
  

  Bccessor*aa=new Bccessor(100);
  Bccessor*bb=new Bccessor(*aa);
  delete aa;
  delete bb;

  Cccessor*aaa=new Cccessor(100);
  Cccessor*abb=new Cccessor(*aaa);
  delete aaa;
  delete abb;


  return 0;
}
