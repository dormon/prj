#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>

template <typename TYPE>
struct PointerDecorator {
  using type = TYPE*;
};

template <typename TYPE>
struct SharedPointerDecorator {
  using type = std::shared_ptr<TYPE>;
};

template <template <typename...> class CONTAINER = std::set,
          template <typename...> class DECORATOR = SharedPointerDecorator>
class Node
    : public CONTAINER<typename DECORATOR<Node<CONTAINER, DECORATOR>>::type> {};

int main() {
  auto root = std::make_shared<Node<>>();

  root->insert(std::make_shared<Node<>>());

  delete new Node<std::vector, PointerDecorator>();
  delete new Node<std::set, PointerDecorator>();

  return 0;
}
