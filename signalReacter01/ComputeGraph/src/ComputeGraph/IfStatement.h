#pragma once

#include <ComputeGraph/Statement.h>

class COMPUTEGRAPH_EXPORT ComputeGraph::IfStatement : public Statement {
  public:
  IfStatement();
  void setConditionVariable(std::shared_ptr<BoolResource> const& v);
  void setStatement(std::shared_ptr<Statement> const& s);
  virtual bool operator()() override;

  protected:
  std::shared_ptr<BoolResource> conditionVariable;
  std::shared_ptr<Statement> statement;
};
