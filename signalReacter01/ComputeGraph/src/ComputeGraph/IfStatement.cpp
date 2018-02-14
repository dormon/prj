#include <ComputeGraph/BoolResource.h>
#include <ComputeGraph/IfStatement.h>

using namespace ComputeGraph;

IfStatement::IfStatement() : Statement(StatementKind::IF) {}

void IfStatement::setConditionVariable(std::shared_ptr<BoolResource> const& v) {
  if (conditionVariable != nullptr) removeSource(&*conditionVariable);
  addSource(&*v);
  conditionVariable = v;
}

void IfStatement::setStatement(std::shared_ptr<Statement> const& s) {
  if (statement != nullptr) removeSource(&*statement);
  addSource(&*s);
  statement = s;
}

void IfStatement::operator()() {
  if (!recompute) return;
  recompute = false;
  if (conditionVariable->getData()) (*statement)();
}
