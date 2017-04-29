#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

/* sometimes you need to copy arguments into std::vector
 * variadic templates allows it
 * you have to specify terminating function that takes zero arguments
 */

/* terminating function take zero arguments and does not modify final vector
 */
template<typename T>
void argsToVector(std::vector<T>&){
}

/* first argument is vector
 * second argument is HEAD of argument pack
 * third argument is TAIL of argument pack
 *
 */
template<typename T,typename... Args>
void argsToVector(std::vector<T>&vec,T const&head,Args... tail){
  vec.push_back(head);//push head into vector
  argsToVector(vec,tail...);//call again on tail
}



/* sometimes you need to copy arguments into array
 * there is small difference with argsToVector
 * you need to know index!
 */

/* terminating function
 */
template<unsigned SIZE,typename TYPE>
void argsToArray(TYPE[SIZE]){
}

/* number of arguments in argument pack can be determined by sizeof...(args) function
 */
template<unsigned SIZE,typename TYPE,typename... Args>
inline void argsToArray(TYPE data[SIZE],TYPE const&v, Args... args){
  data[SIZE-1-sizeof...(args)]=v;
  argsToArray<SIZE,TYPE>(data,args...);
}

int main(){
  std::vector<int>data;
  argsToVector(data,3,2,1,3,4,5);
  for(auto x:data)
    std::cout<<x<<std::endl;

  std::cout<<std::endl;

#define DIMENSION 10
  int data2[DIMENSION];
  argsToArray<DIMENSION>(data2,0,1,2,3,4,5,6,7,8,9);
  for(unsigned i=0;i<DIMENSION;++i)
    std::cout<<data2[i]<<std::endl;

  return 0;
}
