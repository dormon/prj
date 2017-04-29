#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

class Dummy{
  public:
    int asd;
    int*ptr;
    Dummy(int asd){std::cerr<<"volam constructor"<<std::endl;this->asd=asd;this->ptr=new int[10];}
    ~Dummy(){std::cerr<<"volam destructor"<<std::endl;delete[]this->ptr;}
};

template<typename TYPE>
TYPE*newObject(){
  return new TYPE();
}

template<typename TYPE,typename... CONSTRUCTOR_PARAMS>
void callConstructor(char*ptr,CONSTRUCTOR_PARAMS... params){
  new(ptr)TYPE(params...);
}

int main(){

  Dummy*dummy=(Dummy*)new char[sizeof(Dummy)]; //new Dummy(32);
  callConstructor<Dummy,int>((char*)dummy,32);

  //new(dummy)Dummy(32);
  int*ptr=(int*)dummy;
  dummy->~Dummy();
  dummy=NULL;
  delete[]ptr;

  std::string*v=new std::string();
  v->append("asdasdasdasd");
  int*vptr=(int*)v;
  v->~basic_string();
  delete vptr;

  Dummy*b=new Dummy(100);
  b->~Dummy();
  delete(char*)b;

  char*ud=new char[sizeof(unsigned)];
  unsigned val=123;
  callConstructor<unsigned>(ud,val);
//  new(ud)unsigned(val);
  std::cout<<*((unsigned*)(ud))<<std::endl;
  delete[]ud;

  return 0;
}
