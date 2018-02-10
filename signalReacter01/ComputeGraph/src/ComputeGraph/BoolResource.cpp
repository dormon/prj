#include <ComputeGraph/BoolResource.h>

using namespace ComputeGraph;

BoolResource::BoolResource(bool b)
    : Resource(std::make_shared<BoolResourceType>()), data(b) {}

void BoolResource::update(bool b) {
  data = b;
  updateTicks();
}

bool BoolResource::getData() const { return data; }
