#pragma once

#include <Domain/Object.h>

#include <memory>

class domain::InfiniteObject : public Object {
  public:
  std::shared_ptr<Object> inner;
  InfiniteObject(std::string const& n, std::shared_ptr<Object> const& i);
};

inline domain::InfiniteObject::InfiniteObject(std::string const& n,
                                              std::shared_ptr<Object> const& i)
    : Object(ObjectKind::INFINITE, n), inner(i) {}
