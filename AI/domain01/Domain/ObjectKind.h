#pragma once

#include <Domain/Fwd.h>

#include <ostream>

enum class domain::ObjectKind {
  ATOMIC,
  STRUCT,
  INFINITE,
};

inline std::ostream& operator<<(std::ostream& os,
                                domain::ObjectKind const& obj) {
  switch (obj) {
    case domain::ObjectKind::ATOMIC:
      os << "ATOMIC";
      break;
    case domain::ObjectKind::STRUCT:
      os << "STRUCT";
      break;
    case domain::ObjectKind::INFINITE:
      os << "INFINITE";
      break;
  }
  return os;
}
