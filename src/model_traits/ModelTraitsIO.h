#ifndef MODEL_TRAITS_MODEL_TRAITS_IO_H_
#define MODEL_TRAITS_MODEL_TRAITS_IO_H_

#include "ModelTraits.h"
#include <fstream>

namespace mt {
/**
 * @brief Write the model traits file to the stream using the specified backend
 * @tparam Backend the backend policy class e.g., ::YAML
 * @param [in] stream a stream pointing to the appropriate file
 * @return a unique pointer to a filled ModelTraits object
 */
template <typename Backend>
std::unique_ptr<ModelTraits> ReadFromStream(std::istream &stream,
                                            Backend *backend = nullptr);

/**
 * @briefWrite the model traits to a stream using the specified backend
 * @param [in] model_traits the ModelTraits object to write to stream
 * @param [inout] the stream to write to. i.e. a stream to a yaml file
 */
template <typename Backend>
void WriteToStream(const ModelTraits *model_traits, std::ostream &stream,
                   Backend *backend = nullptr);

/**
 * @brief Read model traits from a stream using the specified backend
 */
template <typename Backend>
std::unique_ptr<ModelTraits> ReadFromFile(const std::string &filename,
                                          Backend *backend = nullptr) {
  std::ifstream fstream{filename};
  return ReadFromStream<Backend>(fstream, backend);
}

template <typename Backend>
void WriteToFile(const ModelTraits *model_traits, const std::string &filename,
                 Backend *backend = nullptr) {
  std::ofstream fstream{filename};
  WriteToStream<Backend>(model_traits, fstream, backend);
}
} // namespace mt

#ifdef MT_HAVE_YAML
#include "backends/Yaml.h"
#endif
#ifdef MT_HAVE_SIMMETRIX
#include "backends/Simmetrix.h"
#endif

#endif
