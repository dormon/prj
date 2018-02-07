#pragma once

#include <Domain/AtomicObject.h>
#include <Domain/InfiniteObject.h>
#include <Domain/Object.h>
#include <Domain/StructObject.h>

#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <vector>

class domain::Domain {
  public:
  std::string name;
  std::map<std::string, std::shared_ptr<Object>> objects;
  std::map<std::string, std::shared_ptr<Relation>> relations;
  Domain(std::string const& name, Domain* parent = nullptr);
  void addDomain(std::shared_ptr<Domain> const& d);
  bool hasObject(std::string const& n) const;
  std::shared_ptr<Object> getObject(std::string const& n) const;
  std::shared_ptr<Object> addAtomicObject(std::string const& n);
  std::shared_ptr<Object> addStructObject(
      std::string const& n, std::vector<std::string> const& subNames);
  std::shared_ptr<Object> addInfiniteObject(std::string const& n,
                                            std::string const& subName);
  
  void setParent(Domain* const& p);
  protected:
  Domain* parentDomain;
  std::map<std::string, std::shared_ptr<Domain>> domains;
  std::vector<std::shared_ptr<Object>> getSubObjects(
      std::string const& structName,
      std::vector<std::string> const& subNames) const;
  std::shared_ptr<Object> getSubObject(std::string const& name,
                                       std::string const& subName) const;
  void checkIfSubObjectExist(std::string const& name,
                             std::string const& subName) const;
  void checkIfStructSubObjectsExist(
      std::string const& structName,
      std::vector<std::string> const& subNames) const;
  void checkKind(std::shared_ptr<Object> const& obj,
                 ObjectKind const& kind) const;
  std::shared_ptr<Object> getObjectAndCheckKind(std::string const& name,
                                                ObjectKind const& kind) const;
  void checkInfiniteObject(std::shared_ptr<Object> const& obj,
                           std::string const& subName) const;
  void checkStructObject(std::shared_ptr<Object> const& obj,
                         std::vector<std::string> const& subNames) const;
  void throwDomainAlreadyExists(std::string const& domain) const;
  void throwObjectDoesNotExist(std::string const& objectName) const;
  void throwDifferentKindException(std::string const& objectName,
                                   ObjectKind const& newKind,
                                   ObjectKind const& oldKind) const;
  void throwNoSubObjectException(std::string const& name,
                                 std::string const& subName) const;
  void throwWrongInnerObjectInInfiniteObject(
      std::string const& objName, std::string const& subName,
      std::string const& oldSubName) const;
  void throwWrongNumberOfInnerObject(std::string const& objectName,
                                     size_t nofElements,
                                     size_t oldNofElements) const;
  void throwWrongInnerObjectInStructObject(std::string const& objectName,
                                           size_t id,
                                           std::string const& subName,
                                           std::string const& oldSubName) const;
};

inline domain::Domain::Domain(std::string const& name, Domain* parent)
    : name(name), parentDomain(parent) {}

inline void domain::Domain::setParent(Domain* const& p) { parentDomain = p; }

inline void domain::Domain::addDomain(std::shared_ptr<Domain> const& d) {
  auto const it = domains.find(d->name);
  if (it != domains.end()) {
    if (it->second != d) throwDomainAlreadyExists(d->name);
    return;
  }
  domains[d->name] = d;
}

inline bool domain::Domain::hasObject(std::string const& n) const {
  return objects.count(n) > 0;
}

inline std::shared_ptr<domain::Object> domain::Domain::getObject(
    std::string const& n) const {
  auto it = objects.find(n);
  if (it == objects.end()) throwObjectDoesNotExist(n);
  return it->second;
}

inline void domain::Domain::checkKind(std::shared_ptr<Object> const& obj,
                                      ObjectKind const& kind) const {
  if (obj->kind != ObjectKind::ATOMIC)
    throwDifferentKindException(obj->name, kind, obj->kind);
}

std::shared_ptr<domain::Object> domain::Domain::getObjectAndCheckKind(
    std::string const& name, ObjectKind const& kind) const {
  auto const obj = getObject(name);
  checkKind(obj, kind);
  return obj;
}

inline std::shared_ptr<domain::Object> domain::Domain::addAtomicObject(
    std::string const& n) {
  if (hasObject(n)) {
    auto const obj = getObjectAndCheckKind(n, ObjectKind::ATOMIC);
    return obj;
  }
  auto result = std::make_shared<AtomicObject>(n);
  objects[n] = result;
  return result;
}

inline std::shared_ptr<domain::Object> domain::Domain::addStructObject(
    std::string const& n, std::vector<std::string> const& subNames) {
  if (hasObject(n)) {
    auto const obj = getObjectAndCheckKind(n, ObjectKind::STRUCT);
    checkStructObject(obj, subNames);
    return obj;
  }

  auto const subObjects = getSubObjects(n, subNames);
  auto const result = std::make_shared<StructObject>(n, subObjects);
  objects[n] = result;
  return result;
}

inline std::shared_ptr<domain::Object> domain::Domain::addInfiniteObject(
    std::string const& n, std::string const& subName) {
  if (hasObject(n)) {
    auto const obj = getObjectAndCheckKind(n, ObjectKind::INFINITE);
    checkInfiniteObject(obj, subName);
    return obj;
  }
  auto const subObject = getSubObject(n, subName);
  auto const result = std::make_shared<InfiniteObject>(n, subObject);
  objects[n] = result;
  return result;
}

inline std::shared_ptr<domain::Object> domain::Domain::getSubObject(
    std::string const& name, std::string const& subName) const {
  checkIfSubObjectExist(name, subName);
  return getObject(subName);
}

inline std::vector<std::shared_ptr<domain::Object>>
domain::Domain::getSubObjects(std::string const& structName,
                              std::vector<std::string> const& subNames) const {
  std::vector<std::shared_ptr<Object>> result;
  checkIfStructSubObjectsExist(structName, subNames);
  for (auto const& s : subNames) result.emplace_back(getObject(s));
  return result;
}

inline void domain::Domain::checkInfiniteObject(
    std::shared_ptr<Object> const& obj, std::string const& subName) const {
  auto const typedObj = std::dynamic_pointer_cast<InfiniteObject>(obj);
  if (typedObj->inner->name != subName)
    throwWrongInnerObjectInInfiniteObject(obj->name, subName,
                                          typedObj->inner->name);
}

inline void domain::Domain::checkStructObject(
    std::shared_ptr<Object> const& obj,
    std::vector<std::string> const& subNames) const {
  checkIfStructSubObjectsExist(obj->name, subNames);
  auto const typedObj = std::dynamic_pointer_cast<StructObject>(obj);
  auto const& elements = typedObj->elements;
  if (subNames.size() != elements.size())
    throwWrongNumberOfInnerObject(obj->name, elements.size(), subNames.size());
  for (size_t i = 0; i < subNames.size(); ++i)
    if (elements.at(i)->name != subNames.at(i))
      throwWrongInnerObjectInStructObject(obj->name, i, subNames.at(i),
                                          elements.at(i)->name);
}

inline void domain::Domain::checkIfSubObjectExist(
    std::string const& name, std::string const& subName) const {
  if (!hasObject(subName)) throwNoSubObjectException(name, subName);
}

inline void domain::Domain::checkIfStructSubObjectsExist(
    std::string const& structName,
    std::vector<std::string> const& subNames) const {
  for (auto const& s : subNames) checkIfSubObjectExist(structName, s);
}

inline void domain::Domain::throwDomainAlreadyExists(
    std::string const& domain) const {
  std::stringstream ss;
  ss << "Cannot add domain: \"" << domain << "\" into domain \"";
  ss << name << "\", it already contains domain with that name";
  ss << " and it is different domain";
  throw std::invalid_argument(ss.str());
}

inline void domain::Domain::throwObjectDoesNotExist(
    std::string const& objectName) const {
  std::stringstream ss;
  ss << "Cannot get object: \"" << objectName << "\" from domain \"";
  ss << name << "\", domain does not contain object with that name";
  throw std::invalid_argument(ss.str());
}

inline void domain::Domain::throwDifferentKindException(
    std::string const& objectName, ObjectKind const& newKind,
    ObjectKind const& oldKind) const {
  std::stringstream ss;
  ss << "Cannot add object: \"" << objectName << "\" into domain \"";
  ss << name << "\", it already contains object with that name";
  ss << " and it is not " << newKind << " object, but " << oldKind << " object";
  throw std::invalid_argument(ss.str());
}

inline void domain::Domain::throwNoSubObjectException(
    std::string const& name, std::string const& subName) const {
  std::stringstream ss;
  ss << "Cannot add object: \"" << name << "\" into domain \"";
  ss << name << "\", element of the new object: \"";
  ss << subName << "\" is not registered as object in the domain";
  throw std::invalid_argument(ss.str());
}

inline void domain::Domain::throwWrongInnerObjectInInfiniteObject(
    std::string const& objName, std::string const& subName,
    std::string const& oldSubName) const {
  std::stringstream ss;
  ss << "Cannot add " << ObjectKind::INFINITE << " object: \"" << objName
     << " with inner object: \"" << subName << "\""
     << "\" into domain \"";
  ss << name << "\", it already contains object with that name, but it's inner "
                "object: \""
     << oldSubName << "\" is different";
  throw std::invalid_argument(ss.str());
}

inline void domain::Domain::throwWrongNumberOfInnerObject(
    std::string const& objectName, size_t nofElements,
    size_t oldNofElements) const {
  std::stringstream ss;
  ss << "Cannot add " << ObjectKind::STRUCT << " object: \"" << objectName
     << "with: " << nofElements << "inner objects into domain \"" << name
     << "\", it already contains object with that name, but it has different "
        "number of inner elements: "
     << oldNofElements;
  throw std::invalid_argument(ss.str());
}

inline void domain::Domain::throwWrongInnerObjectInStructObject(
    std::string const& objectName, size_t id, std::string const& subName,
    std::string const& oldSubName) const {
  std::stringstream ss;
  ss << "Cannot add " << ObjectKind::STRUCT << " object: \"" << objectName
     << " into domain \"" << name
     << "\", it already contains object with that name, but it's " << id
     << ". inner object is \"" << oldSubName << "\" not \"" << subName << "\"";
  throw std::invalid_argument(ss.str());
}
