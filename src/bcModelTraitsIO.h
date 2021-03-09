#ifndef BC_MODEL_TRAITS_IO_H__
#define BC_MODEL_TRAITS_IO_H__

#include "bcModelTraits.h"
#include <fstream>

namespace bc {
/*
*/

/**
 * Write the model traits file to the stream using the specified backend
 * @param [in] stream a stream pointing to the appropriate file
 * @return a unique pointer to a filled ModelTraits object
 */
template <typename Backend>
std::unique_ptr<ModelTraits> ReadFromStream(std::istream &stream);

/**
 * Write the model traits file to the stream using the specified backend
 * @param [in] model_traits the ModelTraits object to write to stream
 * @param [inout] the stream to write to. i.e. a stream to a yaml file
 */
template <typename Backend>
void WriteToStream(const ModelTraits *model_traits, std::ostream &stream);

template <typename Backend>
std::unique_ptr<ModelTraits> ReadFromFile(const std::string &filename) {
  std::ifstream fstream{filename};
  return ReadFromStream<Backend>(fstream);
}

template <typename Backend>
void WriteToFile(const ModelTraits *model_traits, const std::string &filename) {
  std::ofstream fstream{filename};
  WriteToStream<Backend>(model_traits, fstream);
}

} // namespace bc

#endif
