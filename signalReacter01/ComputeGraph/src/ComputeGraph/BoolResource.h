#pragma once

#include <ComputeGraph/BoolResourceType.h>
#include <ComputeGraph/Resource.h>

class COMPUTEGRAPH_EXPORT ComputeGraph::BoolResource : public Resource {
  public:
  BoolResource(bool b);
  void update(bool b);
  bool getData() const;

  protected:
  bool data;
};
