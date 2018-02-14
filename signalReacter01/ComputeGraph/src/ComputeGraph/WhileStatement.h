#pragma once

#include<ComputeGraph/BoolResource.h>
#include<ComputeGraph/Statement.h>

class COMPUTEGRAPH_EXPORT ComputeGraph::WhileStatement : public Statement {
  public:
  WhileStatement();
  void setConditionVariable(std::shared_ptr<BoolResource> const& v);
  void setStatement(std::shared_ptr<Statement> const& s);
  virtual void operator()() override;

  protected:
  std::shared_ptr<BoolResource> conditionVariable;
  std::shared_ptr<Statement> statement;
};

