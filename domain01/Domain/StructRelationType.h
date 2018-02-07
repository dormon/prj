#pragma once

#include <Domain/RelationType.h>

#include <memory>
#include <vector>

class domain::StructRelationType : public RelationType {
  public:
  std::vector<std::shared_ptr<RelationType>> inners;
  StructRelationType(std::vector<std::shared_ptr<RelationType>> const& inners);
};

inline domain::StructRelationType::StructRelationType(
    std::vector<std::shared_ptr<RelationType>> const& inners)
    : RelationType(RelationKind::STRUCT), inners(inners) {}
