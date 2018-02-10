#pragma once

#include <ComputeGraph/ResourceTypeKind.h>

class ComputeGraph::ResourceType {
  public:
  ResourceType(ResourceTypeKind const& k);
  ResourceTypeKind getResourceKind() const;
  virtual bool equal(std::shared_ptr<ResourceType> const& other) const = 0;

  protected:
  ResourceTypeKind resourceTypeKind;
};

inline ComputeGraph::ResourceType::ResourceType(ResourceTypeKind const& k)
    : resourceTypeKind(k) {}

inline ComputeGraph::ResourceTypeKind
ComputeGraph::ResourceType::getResourceKind() const {
  return resourceTypeKind;
}
