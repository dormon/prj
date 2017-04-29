#pragma once

#include<vector>
#include<functional>
#include<cassert>

#include<iostream>
#define ___ std::cerr<<__FILE__<<" :"<<__LINE__<<std::endl

class Node{
  public:
    Node(Node*parent = nullptr);
    virtual ~Node();
    virtual void visitor(
        std::function<void(Node*,void*)>const&atEnter = nullptr,
        std::function<void(Node*,void*)>const&atExit = nullptr,
        void*data = nullptr);
    Node*parent = nullptr;
    std::vector<Node*>nodes;
};

inline Node::Node(Node*p):parent(p){}

inline Node::~Node(){
  assert(this!=nullptr);
  for(auto const&x:this->nodes)
    delete x;
}

inline void Node::visitor(
    std::function<void(Node*,void*)>const&atEnter,
    std::function<void(Node*,void*)>const&atExit,
    void*data){
  assert(this!=nullptr);
  if(atEnter)atEnter(this,data);
  for(auto const&x:this->nodes){
    assert(x!=nullptr);
    x->visitor(atEnter,atExit,data);
  }
  if(atExit)atExit(this,data);
}


