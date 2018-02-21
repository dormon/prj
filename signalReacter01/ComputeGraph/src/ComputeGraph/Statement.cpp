#include <ComputeGraph/Statement.h>

using namespace ComputeGraph;

Statement::Statement(StatementKind const& k) : statementKind(k) {}

StatementKind Statement::getStatementKind() const { return statementKind; }

void Statement::react(Signal const& s) {
  switch (s) {
    case Signal::PROPAGATE_RECOMPUTE:
      if (recompute) return;
      recompute = true;
      emit(s);
      break;
  }
}

bool Statement::getRecompute() const { return recompute; }

void Statement::setPrologue(std::shared_ptr<Statement> const& f) {
  if (prologue) removeSource(&*prologue);

  prologue = f;

  if (prologue) addSource(&*prologue);
}

void Statement::computePrologue() {
  if (prologue) (*prologue)();
}

void Statement::operator()() {
  if (!recompute) return;
  recompute = false;
  computePrologue();
  compute();
}
