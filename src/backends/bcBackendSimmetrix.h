#ifndef BC_BACKEND_SIMMETRIX_H__
#define BC_BACKEND_SIMMETRIX_H__
#include "bcModelTraits.h"
#include "bcModelTraitsIO.h"

namespace bc {
//#ifdef BC_HAVE_SIMMETRIX
struct SIMMETRIX : Backend {};

template <>
void WriteToFile<SIMMETRIX>(const ModelTraits *model_traits,
                            const std::string &filename);
template <>
std::unique_ptr<ModelTraits>
ReadFromFile<SIMMETRIX>(const std::string &filename);
//#endif

} // namespace bc

#endif
