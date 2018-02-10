#include <ComputeGraph/BoolResourceType.h>

using namespace ComputeGraph;

BoolResourceType::BoolResourceType() : ResourceType(ResourceTypeKind::BOOL) {}

bool BoolResourceType::equal(std::shared_ptr<ResourceType> const& other) const {
  return other->getResourceKind() == ResourceTypeKind::BOOL;
}
