#pragma once

#include <ComputeGraph/ResourceType.h>
#include <ComputeGraph/Signal.h>
#include <ComputeGraph/SignalReacter.h>
#include <ComputeGraph/computegraph_export.h>

class COMPUTEGRAPH_EXPORT ComputeGraph::Resource
    : public SignalReacter<Signal> {
  public:
  Resource(std::shared_ptr<ResourceType> const& t);
  virtual ~Resource();
  std::shared_ptr<ResourceType> getType() const;
  size_t                        getTicks() const;
  void                          updateTicks();

  protected:
  virtual void react(Signal const& s) override final;
  size_t                        ticks = 1;
  std::shared_ptr<ResourceType> resourceType;
};
