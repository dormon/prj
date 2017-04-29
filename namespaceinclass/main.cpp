#include<iostream>

class MojeTrida{
  public:
    class Initial{
      public:
        int size(){return 0;}
        int bits(){return 32321;}
    }initial;
};

/*
class A{
  protected:
    unsigned _a;
    unsigned _b;
  public:
    A(unsigned a,unsigned b){this->_a = a;this->_b = b;}
};

class B{

}*/


class Buffer{
  //friend class get;
  private:
    unsigned _size  ;
    unsigned _offset;
  public:
    Buffer(unsigned size,unsigned offset){
      this->_size   = size  ;
      this->_offset = offset;
    }
};

int main(){
  MojeTrida asd;
  asd.initial.size();
  Buffer vsd(12,12);
  return 0;
}
