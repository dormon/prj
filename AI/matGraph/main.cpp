#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <tuple>
#include <vector>

class Node {
 public:
 protected:
  std::vector<Node*> parents;
  std::vector<Node*> children;
};

class Graph {
 public:
 protected:
  std::vector<Node*> nodes;
  std::vector<Node*> roots;
};

// Commutation
// a+b == b+a
// +(a,b) == +(b,a)
//
// 1.)
// +abc==+acb==+bac==+bca==+cab==+cba
//
// 2.)
// +abc==+bac && +abc==+acb -> 1.)
//
// 3.)
// +abc

// Association
// (a-b)-c
// a+(b+c)
// ++abc
//
//

class ParameterCommutation{
  public:
    using ParameterID = size_t;
    using GroupID = size_t;
    using Group = std::set<ParameterID>;
    ParameterCommutation(std::vector<Group>const&gs):groups(gs){
      ParameterID nofParameters = 0;
      Group allGroup;
      for(auto const&g:gs){
        nofParameters += g.size();
        for(auto const&x:g)
          allGroup.insert(x);
      }
      if(allGroup.size() != nofParameters)
        throw std::invalid_argument("some parameters are in multiple groups");
      for(auto const&x:allGroup)
        if(x>=nofParameters)
          throw std::invalid_argument("some argument indices are too large");
      parameterToGroupID.resize(nofParameters);
      for(GroupID gid=0;gid<groups.size();++gid)
        for(auto const&p:groups.at(gid))
          parameterToGroupID.at(p) = gid;
    }
    size_t getNofGroups()const{return groups.size();}
    Group const& getGroup(GroupID const&g)const{return groups.at(g);}
    GroupID getParameterGroup(ParameterID const&p)const{return parameterToGroupID.at(p);}
  protected:
    std::vector<Group>groups;
    std::vector<GroupID>parameterToGroupID;
};

class Function {
 public:
  Function(size_t n, ParameterCommutation const& p) : parameterCommutation(p) {
    params.resize(n);
  }
  virtual ~Function() {}
  size_t getNofParameters() const { return params.size(); }
  void bindParameter(size_t i, std::shared_ptr<Function> const& f) {
    params[i] = f;
  }
  std::shared_ptr<Function> getParameter(size_t i) const {
    return params.at(i);
  }
  ParameterCommutation const&getParameterCommutation()const{
    return parameterCommutation;
  }
 protected:
  std::vector<std::shared_ptr<Function>> params;
  ParameterCommutation parameterCommutation;
};

using FunctionID = size_t;
using FunctionPointer = void(*)();

// atomic type
// composize types
// typename - shortcut, abstraction
// type name; - attribute
// Implementation implementation; <- redundant name - type + typename gives full semantic
// FunctionPointer implementation;
// void(*)() implementation;
//
// struct FunctionDescription{
//   Implementation      ; //tags: FunctionPointer
//   NofParameters       ; //tags: size_t, Function
//   ParameterCommutation; //tags: 
// };
//
//
//
//

struct FunctionDescription{
  ParameterCommutation parameterCommutation;
  size_t               nofParameters;
  FunctionPointer      implementation;
};

struct FunctionRegister{
  std::map<FunctionID,FunctionDescription>functions;
};


std::shared_ptr<Function>createFunction(FunctionID const&id);

class Variable : public Function {
 public:
  Variable() : Function(0, {{}}) {}
};

class Addition : public Function {
 public:
   Addition() : Function(2,{{{0,1}}}){}
};

class Multiplication : public Function {
  public:
    Multiplication() : Function(2,{{{0,1}}}){}
};

int main() { return 0; }
