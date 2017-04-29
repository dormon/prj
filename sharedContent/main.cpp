#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

class Data{
  public:
    int*_data;
    int _size;
    Data(int size){
      this->_size = size;
      this->_data = new int[this->_size];
    }
    ~Data(){
      delete[]this->_data;
    }
};

class User{
  public:
    std::shared_ptr<Data*>_data;
    User(std::shared_ptr<Data*>const&data){
      this->_data = data;
    }
    void operator()(){
      std::cout<<(*this->_data)->_size<<std::endl;
    }
};


int main(){
  std::shared_ptr<Data*> data=std::shared_ptr<Data*>((Data**)new char[sizeof(Data)],[=](Data**data){delete *data;delete[]data;});
  *data = new Data(10);
  User user(data);
  user();
  delete*data;
  *data=new Data(20);
  user();
  //delete*data;
  return 0;
}
