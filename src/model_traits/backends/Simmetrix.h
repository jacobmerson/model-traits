#ifndef BC_BACKEND_SIMMETRIX_H__
#define BC_BACKEND_SIMMETRIX_H__
#include "model_traits/ModelTraits.h"
namespace mt {
struct SIMMETRIX {};

// Simmetrix backend specializes reading from file since it cannot be read
// from a stream
template <>
std::unique_ptr<ModelTraits> ReadFromFile(const std::string &filename,
                                          SIMMETRIX *backend);
} // namespace mt

#endif
