#include <ComputeGraph/Statement.h>

using namespace ComputeGraph;

Statement::Statement(StatementKind const& k) : statementKind(k) {}

StatementKind Statement::getStatementKind() const { return statementKind; }

void Statement::react(Signal const& s) {
  switch (s.signalKind) {
    case SignalKind::PROPAGATE_RECOMPUTE:
      if (recompute) return;
      recompute = true;
      emit(s);
      break;
    default:
      break;
  }
}
