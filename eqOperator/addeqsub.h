#pragma once

inline bool operator==(Add const&add,Sub const&sub){
  std::cout<<"Add==Sub"<<std::endl;
  return add._a+add._b==sub._a-sub._b;
}

inline bool operator==(Sub const&sub,Add const&add){
  std::cout<<"Sub==Add"<<std::endl;
  return add==sub;
}


