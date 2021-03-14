#ifndef BC_BACKEND_YAML_CPP_H__
#define BC_BACKEND_YAML_CPP_H__
#include "bcModelTraits.h"
#include "bcModelTraitsIO.h"

namespace mt {
struct YAML : Backend {};

template <>
void WriteToStream(const ModelTraits *model_traits, std::ostream &stream,
                   YAML *backend);
template <>
std::unique_ptr<ModelTraits> ReadFromStream(std::istream &stream,
                                            YAML *backend);

} // namespace mt

#endif
