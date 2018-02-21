#pragma once

#include <ComputeGraph/Fwd.h>

class ComputeGraph::Input {
  public:
  std::shared_ptr<Resource> resource;
  size_t                    seenTicks = 0;
};
