#include <ComputeGraph/WhileStatement.h>

using namespace ComputeGraph;

WhileStatement::WhileStatement() : Statement(StatementKind::WHILE) {}

void WhileStatement::setConditionVariable(
    std::shared_ptr<BoolResource> const& v) {
  if (conditionVariable) removeSource(&*conditionVariable);
  conditionVariable = v;
  if (v) addSource(&*v);
}

void WhileStatement::setStatement(std::shared_ptr<Statement> const& s) {
  if (statement) removeSource(&*statement);
  statement = s;
  if (s) addSource(&*s);
}

std::shared_ptr<Statement> WhileStatement::getStatement() const {
  return statement;
}

std::shared_ptr<BoolResource> WhileStatement::getConditionVariable() const {
  return conditionVariable;
}

void WhileStatement::compute() {
  while (conditionVariable->getData()) (*statement)();
}
