#include <cassert>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <vector>

#include<ComputeGraph/ComputeGraph.h>

/*

enum class CustomResourceKind {
  FLOAT,
};

namespace ComputeGraph{
template <>
inline CustomResourceKind
translateCustomResourceKind<CustomResourceKind, float>() {
  return CustomResourceKind::FLOAT;
}
}

template <typename... T>
std::vector<CustomResourceKind> translateCustomResourceKinds() {
  return std::vector<CustomResourceKind>({ComputeGraph::translateCustomResourceKind<T>()...});
}


template <typename RETURN_TYPE, typename... ARGS>
class Fce : public ComputeGraph::Function<CustomResourceKind> {
  public:
  Fce(std::function<RETURN_TYPE(ARGS...)> const& f)
      : Function(translateCustomResourceKinds<ARGS...>(),
                 {ComputeGraph::translateCustomResourceKind<RETURN_TYPE>()}),
        impl(f) {}
  std::function<RETURN_TYPE(ARGS...)> impl;
  template <size_t... I>
  void uber(std::index_sequence<I...>) {
    std::dynamic_pointer_cast<ComputeGraph::CustomResource<RETURN_TYPE>>(getOutputResource(0))
        ->update(impl(
            std::dynamic_pointer_cast<ComputeGraph::CustomResource<ARGS>>(getInputResource(I))
                ->data...));
  }
  virtual void compute() override {
    uber(std::make_index_sequence<sizeof...(ARGS)>{});
  }
};

*/


int main(){
  return 0;
}

/*
template <typename RETURN_TYPE, typename... ARGS>
std::shared_ptr<Function> createFunction(RETURN_TYPE (*fce)(ARGS...)) {
  return std::make_shared<Fce<RETURN_TYPE, ARGS...>>(fce);
}

template <typename RETURN_TYPE, typename... ARGS>
std::shared_ptr<Function> createFunction(
    std::function<RETURN_TYPE(ARGS...)> const& f) {
  return std::make_shared<Fce<RETURN_TYPE, ARGS...>>(f);
}

#define BINARY_OPERATOR(OPERATOR, OUTPUT_TYPE, INPUT0_TYPE, INPUT1_TYPE)       \
  std::dynamic_pointer_cast<AtomicResource<OUTPUT_TYPE>>(getOutputResource(0)) \
      ->update(std::dynamic_pointer_cast<AtomicResource<INPUT0_TYPE>>(         \
                   getInputResource(0))                                        \
                   ->data OPERATOR                                             \
                       std::dynamic_pointer_cast<AtomicResource<INPUT1_TYPE>>( \
                           getInputResource(1))                                \
                   ->data)

#define BINARY_FUNCTION(CLASS_NAME, OPERATOR, OUTPUT_TYPE, INPUT0_TYPE, \
                        INPUT1_TYPE)                                    \
  class CLASS_NAME : public Function {                                  \
public:                                                                 \
    CLASS_NAME()                                                        \
        : Function({translateCustomResourceKind<INPUT0_TYPE>(),         \
                    translateCustomResourceKind<INPUT1_TYPE>()},        \
                   {translateCustomResourceKind<OUTPUT_TYPE>()}) {}     \
    virtual void compute() override {                                   \
      BINARY_OPERATOR(OPERATOR, OUTPUT_TYPE, INPUT0_TYPE, INPUT1_TYPE); \
    }                                                                   \
  }

BINARY_FUNCTION(AddFloat, +, float, float, float);
BINARY_FUNCTION(SubFloat, -, float, float, float);
BINARY_FUNCTION(MulFloat, *, float, float, float);
BINARY_FUNCTION(DivFloat, /, float, float, float);
BINARY_FUNCTION(LessFloat, <, bool, float, float);
BINARY_FUNCTION(GreaterFloat, >, bool, float, float);
BINARY_FUNCTION(LequalFloat, <=, bool, float, float);
BINARY_FUNCTION(GequalFloat, >=, bool, float, float);
BINARY_FUNCTION(EqualFloat, ==, bool, float, float);
BINARY_FUNCTION(And, &&, bool, bool, bool);
BINARY_FUNCTION(Or, ||, bool, bool, bool);

template <typename T>
std::shared_ptr<AtomicResource<T>> createVar(T const& d) {
  return std::make_shared<AtomicResource<T>>(d);
}

#define DEFINE_OPERATOR(OPERATOR, FUNCTION_NAME, TYPE)                 \
  std::shared_ptr<Function> operator OPERATOR(                         \
      std::shared_ptr<AtomicResource<TYPE>> const& a,                  \
      std::shared_ptr<AtomicResource<TYPE>> const& b) {                \
    auto r = std::make_shared<FUNCTION_NAME>();                        \
    r->bindInput(0, a);                                                \
    r->bindInput(1, b);                                                \
    return r;                                                          \
  }                                                                    \
  std::shared_ptr<Function> operator OPERATOR(                         \
      std::shared_ptr<AtomicResource<TYPE>> const& a, TYPE const& b) { \
    auto r = std::make_shared<FUNCTION_NAME>();                        \
    r->bindInput(0, a);                                                \
    r->bindInput(1, createVar<TYPE>(b));                               \
    return r;                                                          \
  }                                                                    \
  std::shared_ptr<Function> operator OPERATOR(                         \
      TYPE const& a, std::shared_ptr<AtomicResource<TYPE>> const& b) { \
    auto r = std::make_shared<FUNCTION_NAME>();                        \
    r->bindInput(0, createVar<TYPE>(a));                               \
    r->bindInput(1, b);                                                \
    return r;                                                          \
  }

DEFINE_OPERATOR(+, AddFloat, float);
DEFINE_OPERATOR(<, LessFloat, float);

float add(float a, float b) { return a + b; }

int main() {
  createFunction(add);
  createFunction(+[](float a, float b) -> float { return a + b; });

  {
    auto a = createVar<float>(1.f);
    auto b = createVar<float>(2.f);
    auto c = createVar<float>(0.f);
    auto add = a + b;
    add->bindOutput(0, c);
    (*add)();
    if (c->data != 3.f) std::cerr << "Add failed" << std::endl;
  }
  {
    auto acc = createVar<float>(0.f);
    auto it = createVar<float>(0.f);
    auto cmp = createVar<bool>(false);

    auto inc = it + 1.f;
    inc->bindOutput(0, it);

    auto add = acc + it;
    add->bindOutput(0, acc);

    auto less0 = it < 10.f;
    less0->bindOutput(0, cmp);

    auto less1 = it < 10.f;
    less1->bindOutput(0, cmp);

    auto whlBody = std::make_shared<StatementList>();
    whlBody->addStatement(add);
    whlBody->addStatement(inc);
    whlBody->addStatement(less1);

    auto whl = std::make_shared<WhileStatement>();
    whl->setConditionVariable(cmp);
    whl->setStatement(whlBody);

    auto lst = std::make_shared<StatementList>();
    lst->addStatement(less0);
    lst->addStatement(whl);

    (*lst)();

    if (acc->data != 45.f)
      std::cerr << "While failed: " << acc->data << std::endl;
  }
  {
    std::stringstream redirectStream;
    std::streambuf* oldbuf = std::cout.rdbuf(redirectStream.rdbuf());

    auto add = createFunction(+[](float a, float b) -> float {
      std::cout << "a";
      return a + b;
    });
    auto a = createVar<float>(1.f);
    add->bindInput(0, a);
    add->bindInput(1, a);
    add->bindOutput(0, createVar<float>(0.f));
    auto add2 = std::make_shared<AddFloat>();
    add2->bindInput(0, add, 0);
    add2->bindInput(1, add, 0);
    auto c = createVar<float>(0.f);
    add2->bindOutput(0, c);
    (*add2)();
    (*add2)();
    (*add2)();

    std::cout.rdbuf(oldbuf);
    if (redirectStream.str() != "a")
      std::cerr << "Single input invocation: " << redirectStream.str()
                << std::endl;
  }
  {
    std::stringstream redirectStream;
    std::streambuf* oldbuf = std::cout.rdbuf(redirectStream.rdbuf());

    auto add = createFunction(+[](float a, float b) -> float {
      std::cout << "a";
      return a + b;
    });
    auto a = createVar<float>(1.f);
    add->bindInput(0, a);
    add->bindInput(1, a);
    add->bindOutput(0, createVar<float>(0.f));
    auto add2 = std::make_shared<AddFloat>();
    add2->bindInput(0, add, 0);
    add2->bindInput(1, add, 0);
    auto c = createVar<float>(0.f);
    add2->bindOutput(0, c);
    (*add2)();
    (*add2)();
    a->update(2.f);
    (*add2)();
    (*add2)();

    std::cout.rdbuf(oldbuf);
    if (redirectStream.str() != "aa")
      std::cerr << "Update test: " << redirectStream.str() << std::endl;
  }
  {
    std::stringstream redirectStream;
    std::streambuf* oldbuf = std::cout.rdbuf(redirectStream.rdbuf());

    auto add = createFunction(+[](float a, float b) -> float {
      std::cout << "a";
      return a + b;
    });
    auto one = createVar<float>(1.f);
    auto a = createVar<float>(0.f);
    add->bindInput(0, a);
    add->bindInput(1, one);
    add->bindOutput(0, a);
    (*add)();
    (*add)();
    (*add)();
    (*add)();

    std::cout.rdbuf(oldbuf);
    if (redirectStream.str() != "aaaa")
      std::cerr << "Cyclic update test: " << redirectStream.str() << std::endl;
  }
  return 0;
}
*/
