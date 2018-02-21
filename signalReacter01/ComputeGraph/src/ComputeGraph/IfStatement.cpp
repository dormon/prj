#include <ComputeGraph/BoolResource.h>
#include <ComputeGraph/IfStatement.h>

using namespace ComputeGraph;

IfStatement::IfStatement() : Statement(StatementKind::IF) {}

void IfStatement::setConditionVariable(std::shared_ptr<BoolResource> const& v) {
  if (conditionVariable) removeSource(&*conditionVariable);
  conditionVariable = v;
  if (v) addSource(&*v);
}

void IfStatement::setStatement(std::shared_ptr<Statement> const& s) {
  if (statement) removeSource(&*statement);
  statement = s;
  if (s) addSource(&*s);
}

std::shared_ptr<BoolResource> IfStatement::getConditionVariable() const {
  return conditionVariable;
}

std::shared_ptr<Statement> IfStatement::getStatement() const {
  return statement;
}

void IfStatement::compute() {
  if (conditionVariable->getData()) (*statement)();
}
