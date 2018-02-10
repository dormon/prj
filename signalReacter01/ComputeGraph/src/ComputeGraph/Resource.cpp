#include <ComputeGraph/Resource.h>

using namespace ComputeGraph;

Resource::Resource(std::shared_ptr<ResourceType> const& t) : resourceType(t) {}

Resource::~Resource() {}

void Resource::react(Signal const& s) {
  switch (s.signalKind) {
    case SignalKind::PROPAGATE_RECOMPUTE:
      emit(s);
      break;
    default:
      break;
  }
}

std::shared_ptr<ResourceType> Resource::getType() const { return resourceType; }

size_t Resource::getTicks() const { return ticks; }

void Resource::updateTicks(){
  ticks++;
  emit(Signal(SignalKind::PROPAGATE_RECOMPUTE));
}
