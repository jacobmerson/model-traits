#ifndef BC_BACKEND_SIMMETRIX_H__
#define BC_BACKEND_SIMMETRIX_H__
#include "model_traits/ModelTraits.h"
#include "model_traits/ModelTraitsIO.h"

namespace mt {
struct SIMMETRIX {};

template <>
std::unique_ptr<ModelTraits> ReadFromFile(const std::string &filename,
                                          SIMMETRIX *backend);

} // namespace mt

#endif
