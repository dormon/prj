#include<ComputeGraph/StatementList.h>

using namespace ComputeGraph;

StatementList::StatementList() : Statement(StatementKind::STATEMENT_LIST) {}

void StatementList::operator()(){
  if (!recompute)return;
  recompute = false;
  for (auto const& s : statements) (*s)();
}

void StatementList::addStatement(std::shared_ptr<Statement> const& s) {
  addSource(&*s);
  statements.push_back(s);
}

std::vector<std::shared_ptr<Statement>> const& StatementList::getStatements() const {
  return statements;
}
