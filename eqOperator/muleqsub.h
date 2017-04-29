#pragma once

inline bool operator==(Mul const&mul,Sub const&sub){
  std::cout<<"Mul==Sub"<<std::endl;
  return mul._a*mul._b==sub._a-sub._b;
}

inline bool operator==(Sub const&sub,Mul const&mul){
  std::cout<<"Sub==Mul"<<std::endl;
  return mul==sub;
}


