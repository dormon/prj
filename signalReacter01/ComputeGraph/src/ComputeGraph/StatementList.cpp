#include<ComputeGraph/StatementList.h>

using namespace ComputeGraph;

StatementList::StatementList() : Statement(StatementKind::STATEMENT_LIST) {}

bool StatementList::operator()(){
  if (!recompute) return false;
  recompute = false;
  bool isAnyInputChanged = false;
  for (auto const& s : statements) isAnyInputChanged |= (*s)();
  return isAnyInputChanged;
}

void StatementList::addStatement(std::shared_ptr<Statement> const& s) {
  addSource(&*s);
  statements.push_back(s);
}

std::vector<std::shared_ptr<Statement>> const& StatementList::getStatements() const {
  return statements;
}
