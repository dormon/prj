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

enum class TypeKind {
  BOOL,
  CUSTOM,
};

// base class
class Type {
  public:
  Type(TypeKind const& k) : typeKind(k) {}
  virtual ~Type() {}
  virtual bool equal(std::shared_ptr<Type> const& other) const = 0;
  TypeKind getTypeKind() const { return typeKind; }

  protected:
  TypeKind typeKind;
};

// built in type
class BoolType : public Type {
  public:
  BoolType() : Type(TypeKind::BOOL) {}
  virtual bool equal(std::shared_ptr<Type> const& other) const override {
    return other->getTypeKind() == TypeKind::BOOL;
  }
};

class CustomType : public Type {
  public:
  CustomType() : Type(TypeKind::CUSTOM) {}
  virtual bool equal(std::shared_ptr<Type> const& other) const override final {
    if (other->getTypeKind() != TypeKind::CUSTOM) return false;
    return equal(std::dynamic_pointer_cast<CustomType>(other));
  }
  virtual bool equal(
      std::shared_ptr<CustomType> const& other) const = 0;
};

enum class AtomicTypeKind {
  FLOAT,
  UINT32,
};

template <typename T>
AtomicTypeKind translateType();

template <>
inline AtomicTypeKind translateType<float>() {
  return AtomicTypeKind::FLOAT;
}
template <>
inline AtomicTypeKind translateType<uint32_t>() {
  return AtomicTypeKind::UINT32;
}

template <typename T>
class AtomicType : public CustomType {
  public:
  AtomicType() : CustomType(), atomicTypeKind(translateType<T>()) {}
  virtual bool equal(
      std::shared_ptr<CustomType> const& other) const override {
    auto const kinded = std::dynamic_pointer_cast<AtomicType<T>>(other);
    return kinded != nullptr;
  }
  AtomicTypeKind getAtomicTypeKind() const { return atomicTypeKind; }

  protected:
  AtomicTypeKind atomicTypeKind;
};

int main() {
  auto boolType = std::make_shared<BoolType>();
  auto boolType2 = std::make_shared<BoolType>();
  auto floatType = std::make_shared<AtomicType<float>>();
  auto floatType2 = std::make_shared<AtomicType<float>>();
  auto uint32Type = std::make_shared<AtomicType<uint32_t>>();
  auto uint32Type2 = std::make_shared<AtomicType<uint32_t>>();

  std::cout << std::boolalpha;
  std::cout << "boolType  == floatType  : " << boolType->equal(floatType)
            << std::endl;
  std::cout << "boolType  == boolType2  : " << boolType->equal(boolType2)
            << std::endl;
  std::cout << "floatType == uint32Type : " << floatType->equal(uint32Type)
            << std::endl;
  std::cout << "floatType == floatType2 : " << floatType->equal(floatType2)
            << std::endl;
  std::cout << "uint32Type == uint32Type2 : " << uint32Type->equal(uint32Type2)
            << std::endl;

  return 0;
}
