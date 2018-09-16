#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <ComputeGraph/ComputeGraph.h>

using namespace ComputeGraph;

template <typename T>
class AtomicResourceType : public ResourceType {
  public:
  bool equal(std::shared_ptr<ResourceType> const& other) const override {
    auto const typedOther =
        std::dynamic_pointer_cast<AtomicResourceType<T>>(other);
    return typedOther != nullptr;
  }
};

template <typename T,
          typename std::enable_if_t<std::is_same<T, bool>::value, char> = 0>
std::shared_ptr<ResourceType> createType() {
  return std::make_shared<BoolResourceType>();
}

template <typename T,
          typename std::enable_if_t<!std::is_same<T, bool>::value, char> = 0>
std::shared_ptr<ResourceType> createType() {
  return std::make_shared<AtomicResourceType<T>>();
}

template <typename T>
class AtomicResource : public Resource {
  public:
  AtomicResource(T const& d)
      : Resource(std::make_shared<AtomicResourceType<T>>()), data(d) {}
  void update(T d) {
    data = d;
    updateTicks();
  }
  T const& getData() const { return data; }

  protected:
  T data;
};

template <typename T,
          typename std::enable_if_t<std::is_same<T, bool>::value, char> = 0>
std::shared_ptr<BoolResource> convertResource(
    std::shared_ptr<Resource> const& r) {
  return std::dynamic_pointer_cast<BoolResource>(r);
}

template <typename T,
          typename std::enable_if_t<!std::is_same<T, bool>::value, char> = 0>
std::shared_ptr<AtomicResource<T>> convertResource(
    std::shared_ptr<Resource> const& r) {
  return std::dynamic_pointer_cast<AtomicResource<T>>(r);
}

enum class FunctionName {
  ADD_FLOAT,
  LESS_FLOAT,
};

#define DEFINE_BINARY_OPERATOR(NAME, OPERATOR, INPUT0, INPUT1, OUTPUT) \
  class NAME : public Function {                                       \
public:                                                                \
    NAME(size_t* c = nullptr)                                          \
        : Function(                                                    \
              {                                                        \
                  createType<INPUT0>(), createType<INPUT1>(),          \
              },                                                       \
              {                                                        \
                  createType<OUTPUT>(),                                \
              }),                                                      \
          counter(c) {}                                                \
    virtual void execute() override {                                  \
      if (counter != nullptr) ++(*counter);                            \
      auto output = convertResource<OUTPUT>(getOutputResource(0));     \
      auto inputA = convertResource<INPUT0>(getInputResource(0));      \
      auto inputB = convertResource<INPUT1>(getInputResource(1));      \
      output->update(inputA->getData() OPERATOR inputB->getData());    \
    }                                                                  \
    size_t* counter;                                                   \
  }

DEFINE_BINARY_OPERATOR(AddFloat , +, float, float, float);
DEFINE_BINARY_OPERATOR(LessFloat, <, float, float, bool );

template <typename T,
          typename std::enable_if_t<std::is_same<T, bool>::value, char> = 0>
std::shared_ptr<BoolResource> createVar(T const& v) {
  return std::make_shared<BoolResource>(v);
}

template <typename T,
          typename std::enable_if_t<!std::is_same<T, bool>::value, char> = 0>
std::shared_ptr<Resource> createVar(T const& v) {
  return std::make_shared<AtomicResource<T>>(v);
}

template <FunctionName>
std::shared_ptr<Function> createFunction(size_t* counter = nullptr);

template <>
std::shared_ptr<Function> createFunction<FunctionName::ADD_FLOAT>(
    size_t* counter) {
  return std::make_shared<AddFloat>(counter);
}

template <>
std::shared_ptr<Function> createFunction<FunctionName::LESS_FLOAT>(
    size_t* counter) {
  return std::make_shared<LessFloat>(counter);
}

template <typename T,
          typename std::enable_if_t<std::is_same<T, bool>::value, char> = 0>
T getOutput(std::shared_ptr<Function> const& f, size_t i = 0) {
  return std::dynamic_pointer_cast<BoolResource>(f->getOutputResource(i))
      ->getData();
}

template <typename T,
          typename std::enable_if_t<!std::is_same<T, bool>::value, char> = 0>
T getOutput(std::shared_ptr<Function> const& f, size_t i = 0) {
  return std::dynamic_pointer_cast<AtomicResource<T>>(f->getOutputResource(i))
      ->getData();
}

template <typename T>
void updateVar(std::shared_ptr<Resource> const& r, T const& d = 0) {
  convertResource<T>(r)->update(d);
}

SCENARIO("add test") {
  auto a = createVar<float>(2.f);
  auto b = createVar<float>(3.f);
  auto c = createVar<float>(3.f);

  size_t counter = 0;
  auto   add     = createFunction<FunctionName::ADD_FLOAT>(&counter);

  add->bindInput(0, a);
  add->bindInput(1, b);
  add->bindOutput(0, c);

  REQUIRE(a->getTicks() == 1);
  REQUIRE(b->getTicks() == 1);
  REQUIRE(c->getTicks() == 1);
  REQUIRE(add->getInputTicks(0) == 0);
  REQUIRE(add->getInputTicks(1) == 0);

  (*add)();

  REQUIRE(getOutput<float>(add) == 5.f);
  REQUIRE(counter == 1);
  REQUIRE(a->getTicks() == 1);
  REQUIRE(b->getTicks() == 1);
  REQUIRE(c->getTicks() == 2);
  REQUIRE(add->getInputTicks(0) == 1);
  REQUIRE(add->getInputTicks(1) == 1);

  (*add)();

  REQUIRE(getOutput<float>(add) == 5.f);
  REQUIRE(counter == 1);
  REQUIRE(a->getTicks() == 1);
  REQUIRE(b->getTicks() == 1);
  REQUIRE(c->getTicks() == 2);
  REQUIRE(add->getInputTicks(0) == 1);
  REQUIRE(add->getInputTicks(1) == 1);

  (*add)();

  REQUIRE(getOutput<float>(add) == 5.f);
  REQUIRE(counter == 1);
  REQUIRE(a->getTicks() == 1);
  REQUIRE(b->getTicks() == 1);
  REQUIRE(c->getTicks() == 2);
  REQUIRE(add->getInputTicks(0) == 1);
  REQUIRE(add->getInputTicks(1) == 1);
}

SCENARIO("add2 test") {
  auto a = createVar<float>(1.f);
  auto b = createVar<float>(0.f);
  auto c = createVar<float>(0.f);

  size_t counter0 = 0;
  size_t counter1 = 0;

  auto add0 = createFunction<FunctionName::ADD_FLOAT>(&counter0);
  auto add1 = createFunction<FunctionName::ADD_FLOAT>(&counter1);

  add0->bindInput(0, a);
  add0->bindInput(1, a);
  add0->bindOutput(0, b);

  add1->bindInput(0, b);
  add1->bindInput(1, b);
  add1->bindOutput(0, c);
  add1->setPrologue(add0);

  REQUIRE(add0->getRecompute() == true);
  REQUIRE(add1->getRecompute() == true);
  REQUIRE(a->getTicks() == 1);
  REQUIRE(b->getTicks() == 1);
  REQUIRE(c->getTicks() == 1);
  REQUIRE(add0->getInputTicks(0) == 0);
  REQUIRE(add0->getInputTicks(1) == 0);
  REQUIRE(add1->getInputTicks(0) == 0);
  REQUIRE(add1->getInputTicks(1) == 0);

  (*add1)();

  REQUIRE(add0->getRecompute() == false);
  REQUIRE(add1->getRecompute() == true);
  REQUIRE(getOutput<float>(add1) == 4.f);
  REQUIRE(a->getTicks() == 1);
  REQUIRE(b->getTicks() == 2);
  REQUIRE(c->getTicks() == 2);
  REQUIRE(add0->getInputTicks(0) == 1);
  REQUIRE(add0->getInputTicks(1) == 1);
  REQUIRE(add1->getInputTicks(0) == 2);
  REQUIRE(add1->getInputTicks(1) == 2);
  REQUIRE(counter0 == 1);
  REQUIRE(counter1 == 1);

  (*add1)();

  REQUIRE(add0->getRecompute() == false);
  REQUIRE(add1->getRecompute() == false);
  REQUIRE(getOutput<float>(add1) == 4.f);
  REQUIRE(a->getTicks() == 1);
  REQUIRE(b->getTicks() == 2);
  REQUIRE(c->getTicks() == 2);
  REQUIRE(add0->getInputTicks(0) == 1);
  REQUIRE(add0->getInputTicks(1) == 1);
  REQUIRE(add1->getInputTicks(0) == 2);
  REQUIRE(add1->getInputTicks(1) == 2);
  REQUIRE(counter0 == 1);
  REQUIRE(counter1 == 1);

  updateVar(a, 3.f);

  REQUIRE(a->getTicks() == 2);
  REQUIRE(b->getTicks() == 2);
  REQUIRE(c->getTicks() == 2);
  REQUIRE(add0->getInputTicks(0) == 1);
  REQUIRE(add0->getInputTicks(1) == 1);
  REQUIRE(add1->getInputTicks(0) == 2);
  REQUIRE(add1->getInputTicks(1) == 2);
  REQUIRE(add0->getRecompute() == true);
  REQUIRE(add1->getRecompute() == true);

  (*add1)();

  REQUIRE(add0->getRecompute() == false);
  REQUIRE(add1->getRecompute() == true);
  REQUIRE(getOutput<float>(add1) == 12.f);
  REQUIRE(a->getTicks() == 2);
  REQUIRE(b->getTicks() == 3);
  REQUIRE(c->getTicks() == 3);
  REQUIRE(add0->getInputTicks(0) == 2);
  REQUIRE(add0->getInputTicks(1) == 2);
  REQUIRE(add1->getInputTicks(0) == 3);
  REQUIRE(add1->getInputTicks(1) == 3);
  REQUIRE(counter0 == 2);
  REQUIRE(counter1 == 2);

  (*add1)();

  REQUIRE(add0->getRecompute() == false);
  REQUIRE(add1->getRecompute() == false);
  REQUIRE(getOutput<float>(add1) == 12.f);
  REQUIRE(a->getTicks() == 2);
  REQUIRE(b->getTicks() == 3);
  REQUIRE(c->getTicks() == 3);
  REQUIRE(add0->getInputTicks(0) == 2);
  REQUIRE(add0->getInputTicks(1) == 2);
  REQUIRE(add1->getInputTicks(0) == 3);
  REQUIRE(add1->getInputTicks(1) == 3);
  REQUIRE(counter0 == 2);
  REQUIRE(counter1 == 2);

  updateVar(b, 10.f);

  REQUIRE(add0->getRecompute() == false);
  REQUIRE(add1->getRecompute() == true);
  REQUIRE(getOutput<float>(add1) == 12.f);
  REQUIRE(a->getTicks() == 2);
  REQUIRE(b->getTicks() == 4);
  REQUIRE(c->getTicks() == 3);
  REQUIRE(add0->getInputTicks(0) == 2);
  REQUIRE(add0->getInputTicks(1) == 2);
  REQUIRE(add1->getInputTicks(0) == 3);
  REQUIRE(add1->getInputTicks(1) == 3);
  REQUIRE(counter0 == 2);
  REQUIRE(counter1 == 2);

  (*add1)();

  REQUIRE(add0->getRecompute() == false);
  REQUIRE(add1->getRecompute() == false);
  REQUIRE(a->getTicks() == 2);
  REQUIRE(b->getTicks() == 4);
  REQUIRE(c->getTicks() == 4);
  REQUIRE(add0->getInputTicks(0) == 2);
  REQUIRE(add0->getInputTicks(1) == 2);
  REQUIRE(add1->getInputTicks(0) == 4);
  REQUIRE(add1->getInputTicks(1) == 4);
  REQUIRE(counter0 == 2);
  REQUIRE(counter1 == 3);
  REQUIRE(getOutput<float>(add1) == 20.f);
}

SCENARIO("add recurrent test") {
  auto   a        = createVar<float>(0.f);
  auto   b        = createVar<float>(1.f);
  size_t counter0 = 0;

  auto add0 = createFunction<FunctionName::ADD_FLOAT>(&counter0);

  add0->bindInput(0, a);
  add0->bindInput(1, b);
  add0->bindOutput(0, a);

  REQUIRE(a->getTicks() == 1);
  REQUIRE(b->getTicks() == 1);
  REQUIRE(add0->getInputTicks(0) == 0);
  REQUIRE(add0->getInputTicks(1) == 0);
  REQUIRE(add0->getRecompute() == true);

  (*add0)();

  REQUIRE(a->getTicks() == 2);
  REQUIRE(b->getTicks() == 1);
  REQUIRE(add0->getInputTicks(0) == 1);
  REQUIRE(add0->getInputTicks(1) == 1);
  REQUIRE(add0->getRecompute() == true);
  REQUIRE(getOutput<float>(add0) == 1.f);
  REQUIRE(counter0 == 1);

  (*add0)();

  REQUIRE(a->getTicks() == 3);
  REQUIRE(b->getTicks() == 1);
  REQUIRE(add0->getInputTicks(0) == 2);
  REQUIRE(add0->getInputTicks(1) == 1);
  REQUIRE(add0->getRecompute() == true);
  REQUIRE(add0->getRecompute() == true);
  REQUIRE(getOutput<float>(add0) == 2.f);
  REQUIRE(counter0 == 2);

  (*add0)();

  REQUIRE(a->getTicks() == 4);
  REQUIRE(b->getTicks() == 1);
  REQUIRE(add0->getInputTicks(0) == 3);
  REQUIRE(add0->getInputTicks(1) == 1);
  REQUIRE(add0->getRecompute() == true);
  REQUIRE(getOutput<float>(add0) == 3.f);
  REQUIRE(counter0 == 3);
}

SCENARIO("if test") {
  auto   a        = createVar<float>(1.f);
  auto   c        = createVar<float>(0.f);
  auto   b        = createVar<bool>(true);
  size_t counter0 = 0;

  auto add0 = createFunction<FunctionName::ADD_FLOAT>(&counter0);

  add0->bindInput(0, a);
  add0->bindInput(1, a);
  add0->bindOutput(0, c);

  auto if0 = std::make_shared<IfStatement>();
  if0->setStatement(add0);
  if0->setConditionVariable(b);

  REQUIRE(add0->getRecompute() == true);
  REQUIRE(if0->getRecompute() == true);

  (*if0)();

  REQUIRE(add0->getRecompute() == false);
  REQUIRE(if0->getRecompute() == false);
  REQUIRE(convertResource<float>(c)->getData() == 2.f);
  REQUIRE(counter0 == 1);

  (*add0)();

  REQUIRE(add0->getRecompute() == false);
  REQUIRE(if0->getRecompute() == false);
  REQUIRE(convertResource<float>(c)->getData() == 2.f);
  REQUIRE(counter0 == 1);
}

SCENARIO("bind test") {
  auto a = createVar<float>(1.f);
  auto b = createVar<float>(1.f);

  auto add0 = createFunction<FunctionName::ADD_FLOAT>();

  REQUIRE(add0->getRecompute() == true);
  add0->bindInput(0, a);
  add0->bindInput(1, a);
  add0->bindOutput(0, b);

  REQUIRE(add0->getRecompute() == true);

  (*add0)();

  REQUIRE(add0->getRecompute() == false);

  add0->bindInput(0, nullptr);

  REQUIRE(add0->getRecompute() == false);

  add0->bindInput(0, a);

  REQUIRE(add0->getRecompute() == true);
}

SCENARIO("cyclic recompute propagate") {
  auto a = createVar<float>(1.f);
  auto b = createVar<float>(0.f);

  size_t counter0;
  size_t counter1;
  auto   add0 = createFunction<FunctionName::ADD_FLOAT>(&counter0);

  add0->bindInput(0, a);
  add0->bindInput(1, a);
  add0->bindOutput(0, a);

  auto add1 = createFunction<FunctionName::ADD_FLOAT>(&counter1);

  add1->bindInput(0, a);
  add1->bindInput(1, a);
  add1->bindOutput(0, b);
  add1->setPrologue(add0);

  REQUIRE(add0->getRecompute() == true);
  REQUIRE(add1->getRecompute() == true);

  (*add1)();

  REQUIRE(add0->getRecompute() == true);
  REQUIRE(add1->getRecompute() == true);
  REQUIRE(convertResource<float>(b)->getData() == 4.f);

  (*add1)();

  REQUIRE(add0->getRecompute() == true);
  REQUIRE(add1->getRecompute() == true);
  REQUIRE(convertResource<float>(b)->getData() == 8.f);
}

SCENARIO("statement list containing cyclic functions"){
  auto a = createVar<float>(1.f);
  auto add0 = createFunction<FunctionName::ADD_FLOAT>();
  auto add1 = createFunction<FunctionName::ADD_FLOAT>();
  add0->bindInput(0,a);
  add0->bindInput(1,a);
  add0->bindOutput(0,a);
  add1->bindInput(0,a);
  add1->bindInput(1,a);
  add1->bindOutput(0,a);
  auto sl = std::make_shared<StatementList>(add0,add1);

  REQUIRE(add0->getRecompute() == true);
  REQUIRE(add1->getRecompute() == true);
  REQUIRE(sl->getRecompute() == true);

  (*sl)();

  REQUIRE(add0->getRecompute() == true);
  REQUIRE(add1->getRecompute() == true);
  REQUIRE(sl->getRecompute() == true);
  REQUIRE(convertResource<float>(a)->getData() == 4.f);

  (*sl)();

  REQUIRE(add0->getRecompute() == true);
  REQUIRE(add1->getRecompute() == true);
  REQUIRE(sl->getRecompute() == true);
  REQUIRE(convertResource<float>(a)->getData() == 16.f);
}

SCENARIO("statement list containing non cyclic functions"){
  auto a = createVar<float>(1.f);
  auto b = createVar<float>(0.f);
  auto c = createVar<float>(0.f);
  auto add0 = createFunction<FunctionName::ADD_FLOAT>();
  auto add1 = createFunction<FunctionName::ADD_FLOAT>();
  add0->bindInput(0,a);
  add0->bindInput(1,a);
  add0->bindOutput(0,b);
  add1->bindInput(0,b);
  add1->bindInput(1,b);
  add1->bindOutput(0,c);
  auto sl = std::make_shared<StatementList>(add0,add1);
  REQUIRE(add0->getRecompute() == true);
  REQUIRE(add1->getRecompute() == true);
  REQUIRE(sl->getRecompute() == true);

  (*sl)();

  REQUIRE(add0->getRecompute() == false);
  REQUIRE(add1->getRecompute() == false);
  REQUIRE(sl->getRecompute() == false);
  REQUIRE(convertResource<float>(c)->getData() == 4.f);

  (*sl)();

  REQUIRE(convertResource<float>(c)->getData() == 4.f);
}

SCENARIO("while test"){
  auto a = createVar<float>(0.f);
  auto i = createVar<float>(0.f);
  auto one = createVar<float>(1.f);
  auto end = createVar<float>(10.f);
  auto c = createVar<bool>(true);

  auto cmp0 = createFunction<FunctionName::LESS_FLOAT>();
  cmp0->bindInput(0,i);
  cmp0->bindInput(1,end);
  cmp0->bindOutput(0,c);

  auto cmp1 = createFunction<FunctionName::LESS_FLOAT>();
  cmp1->bindInput(0,i);
  cmp1->bindInput(1,end);
  cmp1->bindOutput(0,c);

  auto add = createFunction<FunctionName::ADD_FLOAT>();
  add->bindInput(0,i);
  add->bindInput(1,a);
  add->bindOutput(0,a);

  auto inc = createFunction<FunctionName::ADD_FLOAT>();
  inc->bindInput(0,i);
  inc->bindInput(1,one);
  inc->bindOutput(0,i);

  auto lst1 = std::make_shared<StatementList>(add,inc,cmp1);
  auto whl = std::make_shared<WhileStatement>();
  whl->setConditionVariable(c);
  whl->setStatement(lst1);

  auto lst0 = std::make_shared<StatementList>(cmp0,whl);

  (*lst0)();

  REQUIRE(convertResource<float>(a)->getData() == 45.f);
}
