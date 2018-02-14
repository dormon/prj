#pragma once

#include<ComputeGraph/Input.h>

class ComputeGraph::ResourceInput : public Input {
  public:
  std::shared_ptr<Resource> res;
  size_t seenUpdateTicks = 0;
  ResourceInput(std::shared_ptr<Resource> const& r)
      : Input(InputKind::RESOURCE), res(r) {}
  virtual std::shared_ptr<Resource> getResource() const override { return res; }
};

