#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<geGL/geGL.h>
#include<GL/glew.h>
#include"attrib.h"


template<typename TYPE>
TYPE get(std::shared_ptr<TYPE>const&N){
  return *N;
}

template<typename TYPE*>
TYPE get(TYPE*n){
  return *n;
}


int main(){
  return 0;
}





