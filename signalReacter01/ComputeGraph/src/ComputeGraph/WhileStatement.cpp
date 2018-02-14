#include <ComputeGraph/WhileStatement.h>

using namespace ComputeGraph;

WhileStatement::WhileStatement() : Statement(StatementKind::WHILE) {}

void WhileStatement::setConditionVariable(
    std::shared_ptr<BoolResource> const& v) {
  addSource(&*v);
  conditionVariable = v;
}

void WhileStatement::setStatement(std::shared_ptr<Statement> const& s) {
  addSource(&*s);
  statement = s;
}

void WhileStatement::operator()() {
  if (!recompute) return;
  while (conditionVariable->getData()) (*statement)();
}
