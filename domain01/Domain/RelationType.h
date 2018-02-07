#pragma once

#include <Domain/Fwd.h>
#include <Domain/RelationKind.h>

class domain::RelationType {
  public:
  RelationKind kind;
  RelationType(RelationKind const& k);
};

inline domain::RelationType::RelationType(RelationKind const& k) : kind(k) {}
