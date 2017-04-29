#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

/* Head does not need to be only one argument.
 * it can be composed of multiple argument but there must not be ambiguity
 * for example three function:
 * 1. void function(int,args...)
 * 2. void function(float,args...)
 * 3. void function(int,float,args...)
 * will work on:
 * function(1,2,3,4)
 * function(1.1f,2.2f,3.1f)
 * but it wont work on:
 * function(1,2.3f,2,3)
 * because the compiler cant decide which function it should use (1. or 3.)
 *
 * next function will print pairs of arguments (name,value) of type (std::string,int). if value is not present it will print 0 instead
 * so printDefinitionList("a",3,"b",2,"c","d",5,"e") will print
 * a: 3
 * b: 2
 * c: 0
 * d: 5
 * e: 0
 * it will require two terminating function
 * and it will require forward declaration
 * and we will need to properly distinguish function by their arguments
 */

/* this is terminating function for empty arguments
*/
inline void printDefinitionList(){}

/* this is terminating function if last argument is string
 */
inline void printDefinitionList(std::string const& name){
  std::cout << name << ": 0" << std::endl;
}

/* forward declaration
 */
template<typename...TAIL>
void printDefinitionList(
    std::string const&    ,
    std::string const&    ,
    TAIL        const& ...);

/* this version will print standard pair (name,value)
 */
template<typename...TAIL>
inline void printDefinitionList(
    std::string const&     name ,
    int         const&     value,
    TAIL        const& ... tail ){
  std::cout << name << ": " << value << std::endl;
  printDefinitionList(tail...);
}

// this is wrong because it creates ambiguity
/*
template<typename... ARGS>
inline void printDefinitionList(
    std::string const&    name,
    ARGS        const& ...args){
  std::cout << name << ": 0" << std::endl;
  printDefinitionList(tail...);
}
// */

/* this version will print non standard pair (name,0)
 * please note that it has to be forward declared because it can be used in prior functions
 */
template<typename...TAIL>
inline void printDefinitionList(
    std::string const&     name    ,
    std::string const&     nextName,
    TAIL        const& ... tail    ){
  std::cout << name << ": 0" << std::endl;
  printDefinitionList(nextName,tail...);
}


int main(){
  printDefinitionList(
      "a",3,
      "b",2,
      "c"  ,
      "d",5,
      "e"  );
  return 0;
}
