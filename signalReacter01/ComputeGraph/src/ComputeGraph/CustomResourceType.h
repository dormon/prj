#pragma once

#include <ComputeGraph/ResourceType.h>

class ComputeGraph::CustomResourceType : public ResourceType {
  public:
  CustomResourceType();
  virtual bool equal(
      std::shared_ptr<ResourceType> const& other) const override final;
  virtual bool equal(
      std::shared_ptr<CustomResourceType> const& other) const = 0;
};

ComputeGraph::CustomResourceType::CustomResourceType()
    : ResourceType(ResourceTypeKind::CUSTOM) {}

bool ComputeGraph::CustomResourceType::equal(
    std::shared_ptr<ResourceType> const& other) const {
  if (other->getResourceKind() != ResourceTypeKind::CUSTOM) return false;
  return equal(std::dynamic_pointer_cast<CustomResourceType>(other));
}
