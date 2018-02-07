#pragma once

#include <Domain/Object.h>

#include <iostream>

class domain::AtomicObject : public Object {
  public:
  AtomicObject(std::string const& n);
};

inline domain::AtomicObject::AtomicObject(std::string const& n)
    : Object(ObjectKind::ATOMIC, n) {}
