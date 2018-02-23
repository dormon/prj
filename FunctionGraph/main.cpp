#include <cassert>
#include <chrono>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace FunctionGraph {
class Type;
class EmptyType;
enum class Qualifier;
class UnqualifiedType;
class ParameterType;
class AtomicParameterType;
class StructParameterType;
class InfiniteParameterType;
class AtomicType;
class CompositeType;
class BitType;
class AnyType;
class StructType;
class FunctionType;
class InfiniteType;
class FiniteType;
};

// Convertible: a->b
//
// EmptyType    -> AnyType
// BitType      -> AnyType
// StructType   -> AnyType
// InfiniteType -> AnyType
// FiniteType   -> AnyType
// {A0,A1,...} -> {B0,B1,...} <=> (A0 -> B0) && (A1 -> B1) && ...
// {A}  -> B <=> A -> B
// A    -> {B} <=> A -> B
// A... -> B... <=> A -> B
// A    -> B... <=> A -> B
// {A}  -> B... <=> A -> B
// A[n] -> B... <=> A -> B
// A[n] -> B[m] <=> A -> B && n > m
// A    -> q B  <=> A -> B
// {A0,A1,...} -> {q B0,q B1,..} <=> (A0 -> q B0) && (A1 -> q B1) && ...
// A -> {q B} <=> A -> B
//
//
//
//
//
//
//


class FunctionGraph::Type {
  public:
  virtual ~Type() {}
  virtual bool equal(std::shared_ptr<Type> const& o) const             = 0;
  virtual bool convertibleToThis(std::shared_ptr<Type> const& o) const = 0;
};

class FunctionGraph::EmptyType : public Type {
  public:
  virtual bool equal(std::shared_ptr<Type> const& o) const override {
    return std::dynamic_pointer_cast<EmptyType>(o) != nullptr;
  }
};

enum class FunctionGraph::Qualifier {
  IN,
  OUT,
  INOUT,
};

class FunctionGraph::ParameterType : public Type {};

class FunctionGraph::StructParameterType final : public ParameterType {
  public:
  StructParameterType(std::vector<std::shared_ptr<ParameterType>> const& e)
      : elements(e) {}
  size_t getNofElements() const { return elements.size(); }
  std::shared_ptr<ParameterType> getElement(size_t i) const {
    return elements.at(i);
  }
  bool equal(std::shared_ptr<Type> const& o) const override {
    auto const to = std::dynamic_pointer_cast<StructParameterType>(o);
    if (!to) return false;
    if (to->getNofElements() != getNofElements()) return false;
    for (size_t i = 0; i < getNofElements(); ++i)
      if (!getElement(i)->equal(to->getElement(i))) return false;
    return true;
  }

  bool convertibleToThis(std::shared_ptr<Type> const& o) const override {
    auto const to = std::dynamic_pointer_cast<StructParameterType>(o);
    if (!to) return false;//TODO check StructType
    if (to->getNofElements() != getNofElements()) return false;
    for (size_t i = 0; i < getNofElements(); ++i)
      if (!getElement(i)->convertibleToThis(to->getElement(i))) return false;
    return true;
  }

  protected:
  std::vector<std::shared_ptr<ParameterType>> elements;
};

class FunctionGraph::InfiniteParameterType final : public ParameterType {
  public:
  InfiniteParameterType(std::shared_ptr<ParameterType> const& i) : inner(i) {}
  std::shared_ptr<ParameterType> getInnerType() const { return inner; }
  bool equal(std::shared_ptr<Type> const& o) const override {
    auto const to = std::dynamic_pointer_cast<InfiniteParameterType>(o);
    if (!to) return false;
    return getInnerType()->equal(to->getInnerType());
  }

  bool convertibleToThis(std::shared_ptr<Type>const&o)const override{
    auto const to = std::dynamic_pointer_cast<InfiniteParameterType>(o);
    if (!to) return false; //TODO - finite type check
    return getInnerType()->convertibleToThis(to->getInnerType());
  }

  protected:
  std::shared_ptr<ParameterType> inner;
};

class FunctionGraph::UnqualifiedType : public Type {
  public:
};

class FunctionGraph::AtomicParameterType : public ParameterType {
  public:
  AtomicParameterType(Qualifier const&                        q,
                      std::shared_ptr<UnqualifiedType> const& i)
      : qualifier(q), type(i) {}
  Qualifier                        getQualifier() const { return qualifier; }
  std::shared_ptr<UnqualifiedType> getType() const { return type; }
  virtual bool equal(std::shared_ptr<Type> const& o) const override {
    auto const to = std::dynamic_pointer_cast<AtomicParameterType>(o);
    if (!to) return false;
    return to->qualifier == qualifier && type->equal(to->getType());
  }

  protected:
  Qualifier                        qualifier;
  std::shared_ptr<UnqualifiedType> type;
};

class FunctionGraph::AtomicType : public UnqualifiedType {
  public:
};

class FunctionGraph::AnyType final : public AtomicType {
  public:
  virtual bool equal(std::shared_ptr<Type> const& t) const override {
    return std::dynamic_pointer_cast<AnyType>(t) != nullptr;
  }
  bool convertibleToThis(std::shared_ptr<Type> const&)const override{
    return true;
  }
};

class FunctionGraph::BitType final : public AtomicType {
  public:
  virtual bool equal(std::shared_ptr<Type> const& t) const override {
    return std::dynamic_pointer_cast<BitType>(t) != nullptr;
  }
  bool convertibleToThis(std::shared_ptr<Type> const&t)const override{
    return equal(t);
  }
};

class FunctionGraph::CompositeType : public UnqualifiedType {
  public:
};

class FunctionGraph::StructType final : public CompositeType {
  public:
  StructType(std::vector<std::shared_ptr<UnqualifiedType>> const& t)
      : elements(t) {}
  virtual bool equal(std::shared_ptr<Type> const& t) const override {
    auto const to = std::dynamic_pointer_cast<StructType>(t);
    if (!to) return false;
    if (to->elements.size() != elements.size()) return false;
    for (size_t i = 0; i < elements.size(); ++i)
      if (!elements.at(i)->equal(to->elements.at(i))) return false;
    return true;
  }
  bool convertibleToThis(std::shared_ptr<Type> const& o) const override{
    auto const to = std::dynamic_pointer_cast<StructType>(o);
    if (!to) return false;
    //TODO check for one element structs {float} -> float
    if (to->getNofElements() != getNofElements()) return false;
    for (size_t i = 0; i < getNofElements(); ++i)
      if (!getElement(i)->convertibleToThis(to->getElement(i))) return false;
    return true;
  }
  std::shared_ptr<UnqualifiedType> getElement(size_t i) const {
    return elements.at(i);
  }
  size_t getNofElements() const { return elements.size(); }

  protected:
  std::vector<std::shared_ptr<UnqualifiedType>> elements;
};

class FunctionGraph::FunctionType final : public CompositeType {
  public:
  FunctionType(std::shared_ptr<StructParameterType> const& t) : parameters(t) {}
  virtual bool equal(std::shared_ptr<Type> const& t) const override final {
    auto const to = std::dynamic_pointer_cast<FunctionType>(t);
    if (!to) return false;
    return parameters->equal(to->getParameterTypes());
  }
  std::shared_ptr<StructParameterType> getParameterTypes() const {
    return parameters;
  }
  bool convertibleToThis(std::shared_ptr<Type>const&o)const override{
    auto const to = std::dynamic_pointer_cast<FunctionType>(o);
    if(!to) return false;
    return getParameterTypes()->convertibleToThis(to->getParameterTypes());
  }

  protected:
  std::shared_ptr<StructParameterType> parameters;
};

class FunctionGraph::InfiniteType final : public CompositeType {
  public:
  InfiniteType(std::shared_ptr<UnqualifiedType> const& t) : inner(t) {}
  std::shared_ptr<UnqualifiedType>getInnerType()const{return inner;}
  virtual bool equal(std::shared_ptr<Type> const& t) const override {
    auto const to = std::dynamic_pointer_cast<InfiniteType>(t);
    if (!to) return false;
    return equal(to);
  }
  bool convertibleToThis(std::shared_ptr<Type> const& o)const override{
    auto const to = std::dynamic_pointer_cast<InfiniteType>(o);
    if(!to) return false;//TODO check atomic types and finite types and one element struct types
    return getInnerType()->convertibleToThis(to->getInnerType());
  }

  protected:
  std::shared_ptr<UnqualifiedType> inner;
};

class ResourceType {
  public:
  virtual bool equal(std::shared_ptr<ResourceType> const& other) const = 0;
};

template <typename T>
class AtomicResourceType : public ResourceType {
  public:
  virtual bool equal(
      std::shared_ptr<ResourceType> const& other) const override final {
    return std::dynamic_pointer_cast<AtomicResourceType<T>>(other) != nullptr;
  }
};

class Resource {
  public:
  Resource(std::shared_ptr<ResourceType> const& t) : type(t) {}
  virtual ~Resource() {}
  std::shared_ptr<ResourceType> getType() const { return type; }

  protected:
  std::shared_ptr<ResourceType> type;
};

template <typename T>
class AtomicResource : public Resource {
  public:
  AtomicResource(T const& d)
      : Resource(std::make_shared<AtomicResourceType<T>>()), data(d) {}
  T    getData() const { return data; }
  void updateData(T const& d) { data = d; }

  protected:
  T data;
};

class Function {
  public:
  void bindPin(size_t i, std::shared_ptr<Resource> const& r) {
    if (!pinTypes.at(i)->equal(r->getType()))
      throw std::invalid_argument("pin type is different");

    pinResources.at(i) = r;
  }
  virtual void operator()() = 0;

  protected:
  std::vector<std::shared_ptr<ResourceType>> pinTypes;
  std::vector<std::shared_ptr<Resource>>     pinResources;
};

class AtomicFunction : public Function {
  public:
  // AtomicFunction(size_t n,std::vector<shared_ptr<ResourceType>>):Function(){
  //  pinResources.resize(n);
  //}
};

int main() { return 0; };
