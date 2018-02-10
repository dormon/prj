#pragma once

#include <ComputeGraph/Resource.h>

template <typename T>
class ComputeGraph::CustomResource : public Resource {
  public:
  CustomResource(T const& d);
  void update(T d);
  T const& getData() const;

  protected:
  T data;
};

template <typename T>
inline ComputeGraph::CustomResource<T>::CustomResource(T const& d)
    : Resource(translateCustomResourceKind<T>()), data(d) {}

template <typename T>
inline void ComputeGraph::CustomResource<T>::update(T d) {
  data = d;
  updateTicks++;
  emit(Signal(SignalKind::PROPAGATE_RECOMPUTE));
}

template <typename T>
T const& ComputeGraph::CustomResource<T>::getData() const {
  return data;
}
