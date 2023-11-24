#pragma once
#include <iguana/xml_writer.hpp>

namespace amx {
template <bool pretty = false, typename Stream, typename T>
inline void to_xml(T &&t, Stream &s) {
  iguana::to_xml<pretty>(std::forward<T>(t), s);
}
} 