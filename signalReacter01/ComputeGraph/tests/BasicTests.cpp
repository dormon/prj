#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <ComputeGraph/ComputeGraph.h>

using namespace ComputeGraph;

template <typename T>
class AtomicResourceType : public CustomResourceType {
  public:
  bool equal(std::shared_ptr<CustomResourceType> const& other) const override {
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
    virtual void compute() override {                                  \
      if (counter != nullptr) ++(*counter);                            \
      auto output = convertResource<OUTPUT>(getOutputResource(0));     \
      auto inputA = convertResource<INPUT0>(getInputResource(0));      \
      auto inputB = convertResource<INPUT1>(getInputResource(1));      \
      output->update(inputA->getData() + inputB->getData());           \
    }                                                                  \
    size_t* counter;                                                   \
  }

DEFINE_BINARY_OPERATOR(AddFloat, +, float, float, float);

template <typename T,
          typename std::enable_if_t<std::is_same<T, bool>::value, char> = 0>
std::shared_ptr<Resource> createVar(T const& v) {
  return std::make_shared<BoolResource>(v);
}

template <typename T,
          typename std::enable_if_t<!std::is_same<T, bool>::value, char> = 0>
std::shared_ptr<Resource> createVar(T const& v) {
  return std::make_shared<AtomicResource<T>>(v);
}

template <FunctionName>
std::shared_ptr<Function> createFunction(size_t*counter = nullptr);

template <>
std::shared_ptr<Function> createFunction<FunctionName::ADD_FLOAT>(size_t*counter) {
  return std::make_shared<AddFloat>(counter);
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
  auto add = createFunction<FunctionName::ADD_FLOAT>(&counter);

  add->bindInput(0, a);
  add->bindInput(1, b);
  add->bindOutput(0, c);

  (*add)();

  REQUIRE(getOutput<float>(add) == 5.f);
  REQUIRE(counter == 1);
  (*add)();
  REQUIRE(getOutput<float>(add) == 5.f);
  REQUIRE(counter == 1);
  (*add)();
  REQUIRE(getOutput<float>(add) == 5.f);
  REQUIRE(counter == 1);
}

SCENARIO("add2 test"){
  auto a = createVar<float>(1.f);
  auto b = createVar<float>(0.f);
  auto c = createVar<float>(0.f);

  size_t counter0 = 0;
  size_t counter1 = 0;

  auto add0 = createFunction<FunctionName::ADD_FLOAT>(&counter0);
  auto add1 = createFunction<FunctionName::ADD_FLOAT>(&counter1);

  add0->bindInput(0,a);
  add0->bindInput(1,a);
  add0->bindOutput(0,b);
  
  add1->bindInput(0,add0,0);
  add1->bindInput(1,add0,0);
  add1->bindOutput(0,c);

  (*add1)();

  REQUIRE(getOutput<float>(add1) == 4.f);
  REQUIRE(counter0 == 1);
  REQUIRE(counter1 == 1);

  (*add1)();

  REQUIRE(getOutput<float>(add1) == 4.f);
  REQUIRE(counter0 == 1);
  REQUIRE(counter1 == 1);

  updateVar(a,3.f);

  (*add1)();

  REQUIRE(getOutput<float>(add1) == 12.f);
  REQUIRE(counter0 == 2);
  REQUIRE(counter1 == 2);

  (*add1)();

  REQUIRE(getOutput<float>(add1) == 12.f);
  REQUIRE(counter0 == 2);
  REQUIRE(counter1 == 2);

  updateVar(b,10.f);

  REQUIRE(getOutput<float>(add1) == 12.f);
  REQUIRE(counter0 == 2);
  REQUIRE(counter1 == 2);

  (*add1)();

  REQUIRE(counter0 == 2);
  REQUIRE(counter1 == 3);
  REQUIRE(getOutput<float>(add1) == 20.f);
}
