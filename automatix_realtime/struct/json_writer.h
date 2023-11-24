#pragma once
#include <iguana/json_writer.hpp>

namespace amx {
template <typename Stream, typename T>
IGUANA_INLINE void to_json(Stream &s, T &&t) {
  iguana::to_json(s, t);
}
}
