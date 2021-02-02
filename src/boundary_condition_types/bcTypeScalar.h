#ifndef BC_TYPE_SCALAR_H__
#define BC_TYPE_SCALAR_H__
#include "bcTypedefs.h"

namespace bc {
struct ScalarBC {
  ScalarBC() = default;
  ScalarBC(ScalarType d) noexcept : data(d){};
  operator double() const noexcept { return data; }
  operator double &() noexcept { return data; }
  operator const double &() const noexcept { return data; }
  ScalarType data;
};
} // namespace bc
#endif
