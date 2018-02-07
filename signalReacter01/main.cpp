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

#include <SignalReacter.h>

enum class SignalKind {
  PROPAGATE_RECOMPUTE,
};

class Signal {
  public:
  SignalKind signalKind;
  Signal(SignalKind const& k) : signalKind(k) {}
};

class Function;
class Resource;

class Statement;
class StatementList;

enum class ResourceKind {
  FLOAT,
  BOOL,
};


class Resource : public SignalReacter<Signal> {
  public:
  Resource(ResourceKind const& k) : resourceKind(k) {}
  virtual ~Resource() {}
  virtual void react(Signal const& s) override {
    switch (s.signalKind) {
      case SignalKind::PROPAGATE_RECOMPUTE:
        emit(s);
        break;
      default:
        break;
    }
  }
  ResourceKind getResourceKind() const { return resourceKind; }
  size_t getTicks() const { return updateTicks; }

  protected:
  size_t updateTicks = 1;
  ResourceKind resourceKind;
};

template <typename T>
ResourceKind translateResourceKind();

template <>
inline ResourceKind translateResourceKind<float>() {
  return ResourceKind::FLOAT;
}
template <>
inline ResourceKind translateResourceKind<bool>() {
  return ResourceKind::BOOL;
}

template <typename... T>
std::vector<ResourceKind> translateResourceKinds() {
  return std::vector<ResourceKind>({translateResourceKind<T>()...});
}

template <typename T>
class AtomicResource : public Resource {
  public:
  T data;
  AtomicResource(T const& d) : Resource(translateResourceKind<T>()), data(d) {}
  void update(T d) {
    data = d;
    updateTicks++;
    emit(Signal(SignalKind::PROPAGATE_RECOMPUTE));
  }
};

enum class StatementKind {
  STATEMENT_LIST,
  FUNCTION,
  IF,
  WHILE,
};

class Statement : public SignalReacter<Signal> {
  public:
  Statement(StatementKind const& k) : statementKind(k) {}
  virtual bool operator()() = 0;
  StatementKind getStatementKind() const { return statementKind; }
  virtual void react(Signal const& s) override {
    switch (s.signalKind) {
      case SignalKind::PROPAGATE_RECOMPUTE:
        if (recompute) return;
        recompute = true;
        emit(s);
        break;
      default:
        break;
    }
  }

  protected:
  StatementKind statementKind;
  bool recompute = true;
};

class StatementList : public Statement {
  public:
  StatementList() : Statement(StatementKind::STATEMENT_LIST) {}
  virtual bool operator()() override {
    if (!recompute) return false;
    recompute = false;
    bool isAnyInputChanged = false;
    for (auto const& s : statements) isAnyInputChanged |= (*s)();
    return isAnyInputChanged;
  }
  void addStatement(std::shared_ptr<Statement> const& s) {
    addSource(&*s);
    statements.push_back(s);
  }
  std::vector<std::shared_ptr<Statement>> const& getStatements() const {
    return statements;
  }

  protected:
  std::vector<std::shared_ptr<Statement>> statements;
};

class IfStatement : public Statement {
  public:
  IfStatement() : Statement(StatementKind::IF) {}
  void setConditionVariable(std::shared_ptr<AtomicResource<bool>> const& v) {
    if (conditionVariable != nullptr) removeSource(&*conditionVariable);
    addSource(&*v);
    conditionVariable = v;
  }
  void setStatement(std::shared_ptr<Statement> const& s) {
    if (statement != nullptr) removeSource(&*statement);
    addSource(&*s);
    statement = s;
  }
  virtual bool operator()() override {
    if (!recompute) return false;
    bool isChanged = false;
    if (conditionVariable->data) isChanged |= (*statement)();
    return isChanged;
  }

  protected:
  std::shared_ptr<AtomicResource<bool>> conditionVariable;
  std::shared_ptr<Statement> statement;
};

class WhileStatement : public Statement {
  public:
  WhileStatement() : Statement(StatementKind::WHILE) {}
  void setConditionVariable(std::shared_ptr<AtomicResource<bool>> const& v) {
    addSource(&*v);
    conditionVariable = v;
  }
  void setStatement(std::shared_ptr<Statement> const& s) {
    addSource(&*s);
    statement = s;
  }
  virtual bool operator()() override {
    if (!recompute) return false;
    bool isChanged = false;
    while (conditionVariable->data) isChanged |= (*statement)();
    return isChanged;
  }

  protected:
  std::shared_ptr<AtomicResource<bool>> conditionVariable;
  std::shared_ptr<Statement> statement;
};

enum class InputKind {
  FUNCTION,
  RESOURCE,
};

class Input {
  public:
  Input(InputKind const& k) : inputKind(k) {}
  virtual ~Input() {}
  InputKind getInputKind() const { return inputKind; }
  virtual std::shared_ptr<Resource> getResource() const = 0;

  protected:
  InputKind inputKind;
};

class FunctionInput : public Input {
  public:
  std::shared_ptr<Function> fce;
  size_t outputId;
  FunctionInput(std::shared_ptr<Function> const& f, size_t i)
      : Input(InputKind::FUNCTION), fce(f), outputId(i) {}
  virtual std::shared_ptr<Resource> getResource() const override;
};

class ResourceInput : public Input {
  public:
  std::shared_ptr<Resource> res;
  size_t seenUpdateTicks = 0;
  ResourceInput(std::shared_ptr<Resource> const& r)
      : Input(InputKind::RESOURCE), res(r) {}
  virtual std::shared_ptr<Resource> getResource() const override { return res; }
};

class Function : public Statement {
  public:
  Function(std::vector<ResourceKind> const& i,
           std::vector<ResourceKind> const& o)
      : Statement(StatementKind::FUNCTION),
        inputs(i.size()),
        outputs(o.size()),
        inputTypes(i),
        outputTypes(o) {}
  virtual void compute() = 0;
  virtual void bindInput(size_t i, std::shared_ptr<Function> const& f,
                         size_t j) {
    if (getInputKind(i) != f->getOutputKind(j)) {
      std::stringstream ss;
      ss << "Cannot bind function to input: " << i << " it has diffrent type";
      throw std::invalid_argument(ss.str());
    }
    unbindInput(i);
    if (!f) return;
    addSource(&*f);
    inputs.at(i) = std::make_shared<FunctionInput>(f, j);
  }
  virtual void bindInput(size_t i, std::shared_ptr<Resource> const& r) {
    if (getInputKind(i) != r->getResourceKind()) {
      std::stringstream ss;
      ss << "Cannot bind resource to input: " << i << " it has different type";
      throw std::invalid_argument(ss.str());
    }
    unbindInput(i);
    if (!r) return;
    addSource(&*r);
    inputs.at(i) = std::make_shared<ResourceInput>(r);
  }
  virtual void bindOutput(size_t i, std::shared_ptr<Resource> const& r) {
    if (getOutputKind(i) != r->getResourceKind()) {
      std::stringstream ss;
      ss << "Cannot bind resource to output: " << i << " it has different type";
      throw std::invalid_argument(ss.str());
    }
    unbindOutput(i);
    if (!r) return;
    addTarget(&*r);
    outputs.at(i) = r;
  }

  virtual bool operator()() override {
    if (!recompute) return false;
    recompute = false;
    bool isAnyInputChanged = computeInputs();
    if (!isAnyInputChanged) return false;
    compute();
    // emit(Signal(Signal::UPDATE_TICKS));
    return true;
  }
  std::shared_ptr<Resource> getInputResource(size_t i) const {
    if (inputs.at(i) == nullptr) return nullptr;
    return inputs.at(i)->getResource();
  }
  std::shared_ptr<Resource> getOutputResource(size_t i) const {
    return outputs.at(i);
  }
  ResourceKind getInputKind(size_t i) const { return inputTypes.at(i); }
  ResourceKind getOutputKind(size_t i) const { return outputTypes.at(i); }

  protected:
  bool isOutputDuplicated(size_t id) const {
    if (outputs.at(id) == nullptr) return false;
    auto const output = outputs.at(id);
    for (size_t i = 0; i < outputs.size(); ++i) {
      if (i == id) continue;
      if (outputs.at(i) == nullptr) continue;
      if (outputs.at(i) == output) return true;
    }
    return false;
  }
  void unbindOutput(size_t i) {
    if (outputs.at(i) == nullptr) return;
    if (isOutputDuplicated(i)) {
      outputs.at(i) = nullptr;
      return;
    }
    removeTarget(&*outputs.at(i));
    outputs.at(i) = nullptr;
  }

  bool isInputDuplicated(size_t id) const {
    if (inputs.at(id) == nullptr) return false;
    auto const input = inputs.at(id);
    for (size_t i = 0; i < inputs.size(); ++i) {
      if (i == id) continue;
      if (inputs.at(i) == nullptr) continue;
      if (inputs.at(i)->getInputKind() == input->getInputKind()) {
        switch (input->getInputKind()) {
          case InputKind::RESOURCE:
            if (std::dynamic_pointer_cast<ResourceInput>(input)->res ==
                std::dynamic_pointer_cast<ResourceInput>(inputs.at(i))->res)
              return true;
            break;
          case InputKind::FUNCTION:
            if (std::dynamic_pointer_cast<FunctionInput>(input)->fce ==
                std::dynamic_pointer_cast<FunctionInput>(inputs.at(i))->fce)
              return true;
            break;
        }
      }
    }
    return false;
  }
  void unbindInput(size_t i) {
    if (inputs.at(i) == nullptr) return;
    if (isInputDuplicated(i)) {
      inputs.at(i) = nullptr;
      return;
    }

    auto const input = inputs.at(i);
    switch (input->getInputKind()) {
      case InputKind::RESOURCE:
        removeSource(&*std::dynamic_pointer_cast<ResourceInput>(input)->res);
      case InputKind::FUNCTION:
        removeSource(&*std::dynamic_pointer_cast<FunctionInput>(input)->fce);
    }
    inputs.at(i) = nullptr;
  }
  std::vector<std::shared_ptr<Input>> inputs;
  std::vector<std::shared_ptr<Resource>> outputs;
  std::vector<ResourceKind> inputTypes;
  std::vector<ResourceKind> outputTypes;
  virtual void react(Signal const& s) override {
    switch (s.signalKind) {
      case SignalKind::PROPAGATE_RECOMPUTE:
        if (recompute) return;
        recompute = true;
        emit(s);
        break;
      default:
        break;
    }
  }
  bool computeResourceInput(std::shared_ptr<ResourceInput> const& i) {
    bool changed = i->seenUpdateTicks < i->res->getTicks();
    return changed;
  }
  bool computeFunctionInput(std::shared_ptr<FunctionInput> const& i) {
    return (*i->fce)();
  }
  virtual bool computeInputs() {
    bool isAnyInputChanged = false;
    for (auto const& i : inputs) {
      switch (i->getInputKind()) {
        case InputKind::RESOURCE:
          isAnyInputChanged |=
              computeResourceInput(std::dynamic_pointer_cast<ResourceInput>(i));
          break;
        case InputKind::FUNCTION:
          isAnyInputChanged |=
              computeFunctionInput(std::dynamic_pointer_cast<FunctionInput>(i));
          break;
      }
    }
    return isAnyInputChanged;
  }
};

std::shared_ptr<Resource> FunctionInput::getResource() const {
  return fce->getOutputResource(outputId);
}

template <typename RETURN_TYPE, typename... ARGS>
class Fce : public Function {
  public:
  Fce(std::function<RETURN_TYPE(ARGS...)> const& f)
      : Function(translateResourceKinds<ARGS...>(),
                 {translateResourceKind<RETURN_TYPE>()}),
        impl(f) {}
  std::function<RETURN_TYPE(ARGS...)> impl;
  template <size_t... I>
  void uber(std::index_sequence<I...>) {
    std::dynamic_pointer_cast<AtomicResource<RETURN_TYPE>>(getOutputResource(0))
        ->update(impl(
            std::dynamic_pointer_cast<AtomicResource<ARGS>>(getInputResource(I))
                ->data...));
  }
  virtual void compute() override {
    uber(std::make_index_sequence<sizeof...(ARGS)>{});
  }
};

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
        : Function({translateResourceKind<INPUT0_TYPE>(),               \
                    translateResourceKind<INPUT1_TYPE>()},              \
                   {translateResourceKind<OUTPUT_TYPE>()}) {}           \
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
