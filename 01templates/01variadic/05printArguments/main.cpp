#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

/* variadic templates are powerful extension to templates in c++
 * it can handle functions and classes with varying number of template arguments
 * it always works with HEAD and TAIL concept known from prolog
 * all arguments of templated class are called argument pack = TAIL
 * first argument of argument pack can be viewed as HEAD
 * there has to be terminating function/class that is used if argument pack is empty
 * basic usage is as follows
 *
 * process_args(){//terminating function
 *   
 * }
 *
 * process_args(head,tail){
 *   do_something(head);
 *   process_args(tail);
 * }
 *
 * process_args(1,2,3,"asd",a,3,2,1);
 *
 * let us define print function that prints every argument
 */

//terminating function
void printArguments(){}

template<
  typename   HEAD,
  typename...TAIL>//variadic arguments
void printArguments(
    HEAD const&     head,
    TAIL const& ... tail){//argument pack
  std::cout << head << std::endl;
  printArguments(tail...);
}

int main(){
  //printArguments();
  std::cout<<std::endl;

  printArguments(42);
  std::cout<<std::endl;

  printArguments(1,2,3,"asd",13.3f,'a');
  std::cout<<std::endl;

  printArguments(32,1.1f,"SEMINAR");
  std::cout<<std::endl;

  return 0;
}
