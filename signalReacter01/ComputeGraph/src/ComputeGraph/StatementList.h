#pragma once

#include <ComputeGraph/Statement.h>

class COMPUTEGRAPH_EXPORT ComputeGraph::StatementList : public Statement {
  public:
  StatementList(std::vector<std::shared_ptr<Statement>> const& statements = {});
  template <typename... ARGS>
  StatementList(ARGS const&... args);
  void addStatement(std::shared_ptr<Statement> const& s);
  std::vector<std::shared_ptr<Statement>> const& getStatements() const;

  protected:
  virtual void                            compute() override;
  std::vector<std::shared_ptr<Statement>> statements;
};

template <typename... ARGS>
ComputeGraph::StatementList::StatementList(ARGS const&... args)
    : StatementList(std::vector<std::shared_ptr<Statement>>{args...}) {}
