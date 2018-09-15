#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<functional>
#include<stdexcept>
#include<cassert>
#include<sstream>

#include<Domain/Domain.h>

int main(){
  auto basicDomain = std::make_shared<domain::Domain>("basic");
  return 0;
}
