#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>

using Tag = std::string;

class Type{
  public:
    std::set<Tag>tags;
};

enum class Qualifier{
  IN,
  OUT,
  INOUT,
};

class ParameterType{
  public:
    Qualifier q;
    
};

// function(in T a,
//
//

int main() {
  return 0;
}
