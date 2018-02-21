#pragma once

#include <ComputeGraph/ResourceType.h>

class ComputeGraph::BoolResourceType : public ResourceType {
  public:
  BoolResourceType();
  virtual bool equal(std::shared_ptr<ResourceType> const& other) const override;
};
