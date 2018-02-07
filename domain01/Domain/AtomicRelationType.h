#pragma once

#include <Domain/RelationType.h>

#include <iostream>

class domain::AtomicRelationType : public RelationType {
  public:
  std::string name;
  AtomicRelationType(std::string const& name);
};

inline domain::AtomicRelationType::AtomicRelationType(std::string const& name)
    : RelationType(RelationKind::ATOMIC), name(name) {}
