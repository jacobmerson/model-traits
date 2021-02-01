#ifndef BC_TYPE_SCALAR_H__
#define BC_TYPE_SCALAR_H__
#include "bcTypedefs.h"

namespace bc {
struct ScalarBC {
  ScalarBC(ScalarType d) noexcept : data(d){};
  ScalarType data;
};
} // namespace bc
#endif
