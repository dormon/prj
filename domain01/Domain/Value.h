#pragma once

#include <Domain/Fwd.h>

#include <iostream>

class domain::Value {
  public:
  std::string name;
  Value(std::string const& n);
};

inline domain::Value::Value(std::string const& n) : name(n) {}
