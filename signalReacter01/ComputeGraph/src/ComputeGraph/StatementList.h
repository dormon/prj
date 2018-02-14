#pragma once

#include <ComputeGraph/Statement.h>

class COMPUTEGRAPH_EXPORT ComputeGraph::StatementList : public Statement {
  public:
  StatementList();
  virtual void operator()() override;
  void addStatement(std::shared_ptr<Statement> const& s);
  std::vector<std::shared_ptr<Statement>> const& getStatements() const;

  protected:
  std::vector<std::shared_ptr<Statement>> statements;
};
