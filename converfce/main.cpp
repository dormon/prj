#include<iostream>

/*
void printData(const void*const*data){
  std::cerr<<"korektni verze"<<std::endl;
  std::cout<<data<<std::endl;
}
// */
//*
void printData(const void**data){
  std::cerr<<"nekorektni verze"<<std::endl;
  std::cout<<data<<std::endl;
}
// */


int main(){
  const void**data=NULL;
  ((void(*)(const void**))&printData)(data);
  return 0;
}
