#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<list>
#include<cstring>

template<typename DATA_TYPE, template<typename...>class CHILD_CONTAINER = std::vector>
class Node: public CHILD_CONTAINER<std::shared_ptr<Node<DATA_TYPE,CHILD_CONTAINER>>>{
  public:
    DATA_TYPE data;
};

int main(){
  std::make_shared<Node<int>>();
  std::make_shared<Node<int,std::vector>>();
  std::make_shared<Node<int,std::set>>();
  std::make_shared<Node<int,std::list>>();
  return 0;
}
