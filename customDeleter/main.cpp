#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstdlib>
#include<cstring>

struct Container{
  int*data;
  int size;
  int offset;
  Container(int size){
    this->data   = new int[size];
    this->size   = size;
    this->offset = 0;
    std::memset(this->data,0,this->size*sizeof(int));
  }
  Container(){}
  void free(){
    delete[]this->data;
  }
  Container operator[](unsigned i){
    Container ret;
    ret.size   = this->size-i;
    ret.offset = this->offset+i;
    ret.data   = this->data;
    return ret;
  }
  int&operator*(){
    return this->data[this->offset];
  }
  void print(){
    for(int i=0;i<this->size;++i)
      std::cout<<this->data[i+offset]<<" ";
    std::cout<<std::endl;
  }
};

int main(){
  std::shared_ptr<Container>sc=std::shared_ptr<Container>(new Container(5),[](Container*ptr){ptr->free();delete ptr;});
  Container ssc=(*sc)[1];
  *ssc=32;

  sc->print();
  ssc.print();

  return 0;
}
