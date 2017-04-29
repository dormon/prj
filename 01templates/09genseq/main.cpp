#include<iostream>
#include<vector>

/* gen_seq is useful construction that can generate sequences
 * it can be used to implement reverse operation to argsToVector => vectorToArgs
 * gen_seq is struct that is inherited from itself N times
 * each inheritance adds its number to argument pack
 * last inheritance is from base struct seq<argument pack>
 *
 *
 *
 */

/* base struct has argument pack as template argument
 * if you do this:
 *
 * template<size_t...ARGS>
 * void function(seq<ARGS...>){
 *   something<ARGS...>();
 * }
 *
 * function(seq<0,1,2,3,4>); -> ARGS will contain <0,1,2,3,4>, they will be deduced
 */
template<size_t... Is>
struct seq { };

/* this is templated struct
 * it inherits from itself
 * please note that it has only two template arguments (the last is argument pack)
 * but it inherits from struct that takes 3 template arguments (the last is argument pack)
 * so it will merge last two arguments into argument pack
 */
template<size_t N, size_t... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...> { };

/* specialisation of gen_seq for 0
 * it is terminating inheritance that copies argument pack to seq
 */
template<size_t... Is>
struct gen_seq<0, Is...> : seq<Is...> { };

/* printTemplateList is templated function that prints template arguments 
 */

/* terminating version of printTemplateList for zero template arguments
 */
template<size_t...ARGS>
typename std::enable_if<sizeof...(ARGS)==0,void>::type printTemplateList(){
}

/* this function prints first argument from argument pack and call itself
 *
 */
template<size_t HEAD,size_t... TAIL>
void printTemplateList(){
  std::cout << HEAD << std::endl;
  printTemplateList<TAIL...>();
}

/* printTemplateList2 behaves exactly as printTemplateList but it takes one argument - seq<ARGS...>
 * if you give it seq<0,1,2,3> it will deduce ARGS to be <0,1,2,3>
 */
template<size_t... ARGS>
void printTemplateList2(seq<ARGS...>){
  printTemplateList<ARGS...>();
}

int main(){
  //1. print template arguments
  printTemplateList<0,1,2,3,4>();
  std::cout<<std::endl;

  //1. deduce template arguments
  //2. print template arguments
  printTemplateList2(seq<0,1,2,3,4>{});
  std::cout<<std::endl;

  //1. generate get_seq
  //2. deduce base struct seq<args...>
  //3. deduce template arguments
  //4. print template arguments
  printTemplateList2(gen_seq<100>{});
  std::cout<<std::endl;

  return 0;
}

