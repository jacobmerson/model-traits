#ifndef BC_BACKEND_SIMMETRIX_H__
#define BC_BACKEND_SIMMETRIX_H__
#include "model_traits/bcModelTraits.h"
#include "model_traits/bcModelTraitsIO.h"

namespace mt {
struct SIMMETRIX : Backend {};

template <>
std::unique_ptr<ModelTraits> ReadFromFile(const std::string &filename,
                                          SIMMETRIX *backend);

} // namespace mt

#endif
