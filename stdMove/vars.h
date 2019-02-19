#pragma once

#include<iostream>
#include<vector>

class Vars{
  public:
    Vars(){
      std::cerr << "Vars::Vars()" << std::endl;
    }
    Vars(Vars const&v){
      std::cerr << "Vars::Vars(Vars const&v)" << std::endl;
    }
    Vars(Vars &&v){
      std::cerr << "Vars::Vars(Vars &&v)" << std::endl;
    }
    ~Vars(){
      std::cerr << "Vars::~Vars()" << std::endl;
    }
    std::vector<float>data;
    std::string d;
};

