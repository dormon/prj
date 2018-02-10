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

bool WhileStatement::operator()() {
  if (!recompute) return false;
  bool isChanged = false;
  while (conditionVariable->getData()) isChanged |= (*statement)();
  return isChanged;
}
