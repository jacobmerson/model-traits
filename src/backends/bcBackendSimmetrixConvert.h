#ifndef BC_BACKEND_SIMMETRIX_CONVERT_H
#define BC_BACKEND_SIMMETRIX_CONVERT_H
#include "bcBackendSimmetrix.h"
#include "bcConvert.h"

namespace bc {
template <> struct convert<SIMMETRIX> {
  template <typename R, typename T> static R encode(const T &, SIMMETRIX *);
};
} // namespace bc

#endif
