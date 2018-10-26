#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<functional>

using FCE = int(*)(int,int);

int addImpl(int a,int b){
  return a+b;
}

int subImpl(int a,int b){
  return a-b;
}

FCE add = [](int a,int b){
  return a+b;
};

FCE sub = [](int a,int b){
  return a-b;
};


int main(){

  return 0;
}
