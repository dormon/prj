#pragma once

#include <Domain/Fwd.h>
#include <Domain/ObjectKind.h>

#include <iostream>

class domain::Object {
  public:
  ObjectKind kind;
  std::string name;
  Object(ObjectKind const& k, std::string const& n);
  virtual ~Object();
};

inline domain::Object::Object(ObjectKind const& k, std::string const& n)
    : kind(k), name(n) {}

inline domain::Object::~Object() {}
