#include<iostream>
#include<memory>
#include<glm/glm.hpp>

/* Shared pointer to array
 * You have to specify your destructor
 */

int main(){
  //shared pointer to array of 2 ints
  std::shared_ptr<int>sp = nullptr;
  //now we have to specify destructor
  sp = std::shared_ptr<int>(new int[2],std::default_delete<int[]>());
  (&(*sp))[0]=12;
  (&(*sp))[1]=321;
  int*ss=&(*sp);
  std::cout<<ss[0]<<" "<<ss[1]<<std::endl;


  //more extreme example follows, user discretion advised
  //you can decouple object allocation and object construction
  //and you can also decouple object destruction and object deallocation
  //
  //char*ptr = new char[sizeof(Object)];
  //new(ptr)Object(...);
  //
  //((Object*)ptr)->~Object();
  //delete[]ptr;
  //
  //This is equivalent to:
  //
  //Object*o = new Object(...);
  //
  //delete o;
  //
  //now we want to create shared pointer to array (change char* to std::shared_ptr<char>)
  //Object class is standard std::string for this example

  std::shared_ptr<char>d=std::shared_ptr<char>(new char[sizeof(std::string)],[&](char*ptr){((std::string*)ptr)->~basic_string();delete[]ptr;});
  new(&(*d))std::string("asd");
  std::cout<<*((std::string*)(&(*d)))<<std::endl;

  return 0;
}
