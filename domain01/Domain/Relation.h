#pragma once

#include <Domain/Fwd.h>

#include <iostream>
#include <memory>

class domain::Relation {
  public:
  std::string name;
  std::shared_ptr<RelationType> type;
  Relation(std::string const& n, std::shared_ptr<RelationType> const& t);
};

inline domain::Relation::Relation(std::string const& n,
                                  std::shared_ptr<RelationType> const& t)
    : name(n), type(t) {}
