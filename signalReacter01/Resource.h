#pragma once

#include<SignalReacter.h>

class Resource : public SignalReacter<Signal> {
  public:
    size_t updateTicks = 1;
    Resource(ResourceKind const& k) : resourceKind(k) {}
    virtual ~Resource() {}
    virtual void react(Signal const& s) override {
      switch (s.signalKind) {
        case SignalKind::PROPAGATE_RECOMPUTE:
          emit(s);
          break;
        case SignalKind::UPDATE_TICKS:
          updateTicks++;
          break;
      }
    }
    ResourceKind getResourceKind() const { return resourceKind; }
  protected:
    ResourceKind resourceKind;
};

