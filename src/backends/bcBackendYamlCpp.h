#ifndef BC_BACKEND_YAML_CPP_H__
#define BC_BACKEND_YAML_CPP_H__
#include "bcModelTraits.h"
#include "bcModelTraitsIO.h"

namespace bc {
struct YAML : Backend {};

template <>
void WriteToStream<YAML>(const ModelTraits *model_traits, std::ostream &stream);
template <>
std::unique_ptr<ModelTraits> ReadFromStream<YAML>(std::istream &stream);

} // namespace bc

#endif
