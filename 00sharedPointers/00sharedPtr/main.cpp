#include<iostream>
#include<memory>
#include<glm/glm.hpp>
#include<vector>

class Node{
  public:
    Node(Node const& p = nullptr):parent(p){}
    Node*parent;
    std::vector<std::shared_ptr<Node>>childs;
    float matrix[16];
};

int main(){
  {
    auto n = std::make_shared<Node>();
    auto n0 = std::make_shared<Node>();
    auto n1 = std::make_shared<Node>();
    auto n2 = std::make_shared<Node>();
    n->childs.push_back(n0);
    n->childs.push_back(n1);
    n->childs.push_back(n2);
    n1->childs.push_back(n2);
  }


  //    n
  // n0 n1 n2
  //    n2

  /*
     auto v = new A(10);
     std::vector<A*>vec;
     vec.push_back(v);
     vec.push_back(v);
     vec.push_back(v);
     */
  //for(auto const&x:vec)
  //  delete x;


  return 0;
}
