#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<algorithm>

/* weak pointers can be converted to shared pointer and vice versa
 * weak pointers can be in set
 * you have to specify owner_less in order to be able to put weak pointers into set
 */

int main(){
  //create shared pointer to int
  std::shared_ptr<int>asd=std::make_shared<int>(101);
  //create weak pointer and convert shared pointer to weak pointer
  std::weak_ptr<int>oj=asd;
  //to convert weak pointer to shared pointer you have to call .lock function
  std::shared_ptr<int>ojj=oj.lock();
  std::cout<<*ojj<<std::endl;

  //set of weak pointers is available, but you have to specify owner_less
  std::set<std::weak_ptr<int>,std::owner_less<std::weak_ptr<int>>>ss;
  ss.insert(oj);
  ss.insert(asd);

  //you can use find_if function inside algorithm.h to find specific item inside container
  auto ii=std::find_if(ss.begin(),ss.end(),[&](std::weak_ptr<int>const&ins){return *ins.lock()==101;});
  std::cout<<*ii->lock()<<std::endl;

  return 0;
}
