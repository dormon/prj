#pragma once

#include <Domain/Object.h>

#include <memory>
#include <vector>

class domain::StructObject : public Object {
  public:
  std::vector<std::shared_ptr<Object>> elements;
  StructObject(std::string const& n,
               std::vector<std::shared_ptr<Object>> const& e);
};

domain::StructObject::StructObject(
    std::string const& n, std::vector<std::shared_ptr<Object>> const& e)
    : Object(ObjectKind::STRUCT, n), elements(e) {}
