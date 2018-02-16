#include <ComputeGraph/StatementList.h>

using namespace ComputeGraph;

StatementList::StatementList(
    std::vector<std::shared_ptr<Statement>> const& statements)
    : Statement(StatementKind::STATEMENT_LIST), statements(statements) {
  for (auto const& s : statements) addSource(&*s);
}

void StatementList::compute() {
  for (auto const& s : statements) (*s)();
}

void StatementList::addStatement(std::shared_ptr<Statement> const& s) {
  addSource(&*s);
  statements.push_back(s);
}

std::vector<std::shared_ptr<Statement>> const& StatementList::getStatements()
    const {
  return statements;
}
