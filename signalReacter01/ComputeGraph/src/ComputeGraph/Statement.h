#pragma once

#include <ComputeGraph/Fwd.h>
#include <ComputeGraph/Signal.h>
#include <ComputeGraph/SignalReacter.h>
#include <ComputeGraph/StatementKind.h>
#include <ComputeGraph/computegraph_export.h>

class COMPUTEGRAPH_EXPORT ComputeGraph::Statement
    : public SignalReacter<Signal> {
  public:
  Statement(StatementKind const& k);
  void          operator()();
  StatementKind getStatementKind() const;
  bool          getRecompute() const;
  void          setPrologue(std::shared_ptr<Statement> const&);

  protected:
  virtual void react(Signal const& s) override final;
  virtual void               compute() = 0;
  void                       computePrologue();
  std::shared_ptr<Statement> prologue;
  StatementKind              statementKind;
  bool                       recompute = true;
};
