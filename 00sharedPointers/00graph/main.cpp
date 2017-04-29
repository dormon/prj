#include<iostream>
#include<memory>
#include<glm/glm.hpp>
#include<vector>

/* Shared pointers are useful in situations where object are shared in many location.
 * In those situations, deletion can be tricky for simple pointers
 * Shared pointers have concept call reference counting.
 * This makes deletion very easy
 */


/**
 * @brief This class represents node of generic graph that uses shared pointers to other nodes
 */
class Node: public std::vector<std::weak_ptr<Node>>{
};

/**
 * @brief This class represents node of gegeric graph that uses simple pointers to other nodes
 */
class NodeOld: public std::vector<NodeOld*>{
  public:
    ~NodeOld(){
      for(auto x:*this)//this is problem, nodes can be used elsewhere
        delete x;
    }
};



int main(){
  //this will delete itself without an error
  std::shared_ptr<Node>a = std::make_shared<Node>();
  std::shared_ptr<Node>b = std::make_shared<Node>();
  std::shared_ptr<Node>c = std::make_shared<Node>();
  std::shared_ptr<Node>d = std::make_shared<Node>();
  a->push_back(a);
  a->push_back(b);
  b->push_back(a);
  b->push_back(c);
  b->push_back(d);
  c->push_back(d);
  d->push_back(a);
  d->push_back(d);
  d->push_back(b);
 
  return 0;
}
