#pragma once

#include <ComputeGraph/Fwd.h>
#include <ComputeGraph/computegraph_export.h>
#include <ComputeGraph/Signal.h>
#include <ComputeGraph/SignalReacter.h>
#include <ComputeGraph/StatementKind.h>

class COMPUTEGRAPH_EXPORT ComputeGraph::Statement : public SignalReacter<Signal> {
  public:
  Statement(StatementKind const& k);
  virtual bool operator()() = 0;
  StatementKind getStatementKind() const ;
  virtual void react(Signal const& s) override ;

  protected:
  StatementKind statementKind;
  bool recompute = true;
};
