#pragma once

#include <Domain/RelationType.h>

#include <memory>

class domain::InfiniteRelationType : public RelationType {
  public:
  std::shared_ptr<RelationType> inner;
  InfiniteRelationType(std::shared_ptr<RelationType> const& i);
};

inline domain::InfiniteRelationType::InfiniteRelationType(
    std::shared_ptr<RelationType> const& i)
    : RelationType(RelationKind::INFINITE), inner(i) {}
