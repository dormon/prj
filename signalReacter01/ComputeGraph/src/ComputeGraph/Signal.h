#pragma once

#include<ComputeGraph/SignalKind.h>

class ComputeGraph::Signal {
  public:
  SignalKind signalKind;
  Signal(SignalKind const& k) : signalKind(k) {}
};

