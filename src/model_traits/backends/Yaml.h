#ifndef BC_BACKEND_YAML_CPP_H__
#define BC_BACKEND_YAML_CPP_H__
#include "model_traits/ModelTraits.h"
#include "model_traits/ModelTraitsIO.h"

namespace mt {
struct YAML {};

template <>
void WriteToStream(const ModelTraits *model_traits, std::ostream &stream,
                   YAML *backend);
template <>
std::unique_ptr<ModelTraits> ReadFromStream(std::istream &stream,
                                            YAML *backend);

} // namespace mt

#endif
