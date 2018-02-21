#pragma once

#include <ComputeGraph/Statement.h>

class COMPUTEGRAPH_EXPORT ComputeGraph::IfStatement : public Statement {
  public:
  IfStatement();
  void setConditionVariable(std::shared_ptr<BoolResource> const& v);
  void setStatement(std::shared_ptr<Statement> const& s);

  protected:
  virtual void                  compute() override;
  std::shared_ptr<BoolResource> conditionVariable;
  std::shared_ptr<Statement>    statement;
};
