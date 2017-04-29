#include<iostream>
#include<sstream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

int main(){
  std::stringstream oss;
  auto old = std::cerr.rdbuf( oss.rdbuf() );
  std::cerr << "this goes to cerr";
  std::cout << "[" << oss.str() << "]";
  std::cerr.rdbuf(old);

  return 0;
}
