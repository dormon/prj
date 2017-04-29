#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

/* sometimes you want to extract first value of particular type from argument pack
 * In this file, the function that does that is called valueOfFirstArgumentOfType
 * this function return default value for each type if that type was not found in argument pack
 * valueOfFirstArgumentOfType function loops over argument pack and if it find particular type, it returns its value.
 * If it does not find particular type, it returns default value
 */

/* nullValue return default value for each type
 */
template<typename TO>
TO nullValue();

/* specialisation of nullValue for some types
 */
template<>inline int         nullValue<int        >(){return 0  ;}
template<>inline float       nullValue<float      >(){return 0.f;}
template<>inline const char* nullValue<const char*>(){return "" ;}

/* if argument pack is empty
 * return default value for particular type 
 */
template<typename TO>
inline TO valueOfFirstArgumentOfType(){
  return nullValue<TO>();
}

/* if first argument has correct type
 * return its value
 */
template<typename TARGET_TYPE,typename... TAIL>
inline TARGET_TYPE valueOfFirstArgumentOfType(
    TARGET_TYPE const&     to,
    TAIL        const& ...   ){
  return to;
}

/* please note that enable_if<is_same> has to be used here, otherwise it will create ambiguity with previous function
 */
template<
  typename    TARGET_TYPE,
  typename    HEAD       ,
  typename... TAIL       >
typename std::enable_if<!std::is_same<TARGET_TYPE,HEAD>::value,TARGET_TYPE>::type valueOfFirstArgumentOfType(
    HEAD const&         ,
    TAIL const& ... tail){
  return valueOfFirstArgumentOfType<TARGET_TYPE>(tail...);
}


int main(){
  //we want to extract 23
  int val=valueOfFirstArgumentOfType<int>("asd",'a',"asdas",32,"assss");
  std::cout<<val<<std::endl;
  return 0;
}
