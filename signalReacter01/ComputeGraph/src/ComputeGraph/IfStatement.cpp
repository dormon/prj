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

bool IfStatement::operator()() {
  if (!recompute) return false;
  bool isChanged = false;
  if (conditionVariable->getData()) isChanged |= (*statement)();
  return isChanged;
}
