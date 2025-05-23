#pragma once

#include<Vars/Vars.h>
#include<map>
#include<vector>

class Barrier{
  public:
    Barrier() = delete;
    Barrier(Barrier const&) = delete;
    Barrier(vars::Vars&vars,std::string const&method,std::vector<std::string>const&inputs = {});
    bool notChange();
  protected:
    vars::Vars&vars;
    std::string method;
    std::vector<std::tuple<std::shared_ptr<vars::Resource>,size_t,std::string>>resources;
    bool firstCall = true;

};

class Barrier2{
  public:
    Barrier2() = delete;
    Barrier2(Barrier const&) = delete;
    Barrier2(vars::Vars&vars,std::string const&method,std::vector<std::string>const&inputs = {});
    bool notChange();
  protected:
    vars::Vars&vars;
    std::string method;
    std::vector<std::tuple<std::shared_ptr<vars::Resource>,size_t,std::string>>resources;
    bool firstCall = true;

};

class ObjectData{
  public:
    ObjectData(vars::Vars&vars);
    std::shared_ptr<Barrier> addMethod(std::string const&method,std::vector<std::string>const&inputs);
  protected:
    vars::Vars&vars;
    std::map<std::string,std::shared_ptr<Barrier>>barriers;
};

bool notChanged(
    vars::Vars                   &vars           ,
    std::string             const&objectName     ,
    std::string             const&method         ,
    std::vector<std::string>const&inputs     = {});

bool notChanged(
    vars::Vars                   &vars           ,
    char const* names);

bool notChanged2(
    vars::Vars                   &vars           ,
    char const* names);
