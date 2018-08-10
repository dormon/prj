#include <iostream>
#include <tuple>
#include <sstream>
#include <vector>
#include <memory>
#include <chrono>
#include <functional>
#include <iomanip>
#include <map>
#include <Vars/Vars.h>
#include <Vars/Resource.h>

class Barrier{
  public:
    Barrier(vars::Vars&vars,std::vector<std::string>const&inputs = {}):vars(vars){
      for(auto const&i:inputs){
        if(!vars.has(i))
          throw std::runtime_error(std::string("cannot create Barrier, missing input variable: ")+i);
        resources.push_back(std::tuple<std::shared_ptr<vars::Resource>,size_t>(vars.getResource(i),vars.getTicks(i)));
      }
    }
    bool notChange(){
      bool changed = firstCall;
      firstCall = false;
      for(auto const&r:resources)
        if(std::get<0>(r)->getTicks() > std::get<1>(r)){
          changed |= true;
          break;
        }
      if(changed)
        for(auto &r:resources)
          std::get<1>(r) = std::get<0>(r)->getTicks();
      return !changed;
    }
  protected:
    vars::Vars&vars;
    std::vector<std::tuple<std::shared_ptr<vars::Resource>,size_t>>resources;
    bool firstCall = true;

};


int main() {
  return 0;
}
