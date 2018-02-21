#pragma once

#include <ComputeGraph/Fwd.h>

enum class ComputeGraph::StatementKind {
  STATEMENT_LIST,
  FUNCTION,
  IF,
  WHILE,
};
