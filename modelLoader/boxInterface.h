#pragma once

#include<memory>
#include<map>

#include"pinFactory.h"

class Pin;

class BoxInterface:public std::map<std::string,std::shared_ptr<Pin>>{
  public:
    BoxInterface();
    std::string toStr();
};

