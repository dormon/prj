#pragma once

#include <ComputeGraph/BoolResource.h>
#include <ComputeGraph/Statement.h>

class COMPUTEGRAPH_EXPORT ComputeGraph::WhileStatement : public Statement {
  public:
  WhileStatement();
  void setConditionVariable(std::shared_ptr<BoolResource> const& v);
  void setStatement(std::shared_ptr<Statement> const& s);
  std::shared_ptr<Statement>    getStatement() const;
  std::shared_ptr<BoolResource> getConditionVariable() const;

  protected:
  virtual void                  compute() override;
  std::shared_ptr<BoolResource> conditionVariable;
  std::shared_ptr<Statement>    statement;
};
