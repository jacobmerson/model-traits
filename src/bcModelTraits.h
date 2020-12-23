#ifndef BC_MODEL_TRAITS_H__
#define BC_MODEL_TRAITS_H__
#include "bcCategoryNode.h"
#include "bcINode.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace bc {
class ModelTraits {
public:
  ModelTraits(const std::string &name);
  /**
   * Adds a case if it doesn't exist and returns a pointer
   * to it's INode. If the case does exist, the pointer to
   * the case is returned.
   */
  INode *AddCase(const std::string &name);
  /**
   * Removes a case from ModelTraits by it's name
   */
  bool RemoveCase(const std::string &name);

protected:
  std::string name_;
  std::unordered_map<std::string, std::unique_ptr<CategoryNode>> cases_;
};

/*
// Eventually I would like to add these convenience functions where
// the user can register new backends and these functions will choose the
// correct backend at runtime...actually, this might not be the right choice
// because the user may want to read from one backend and write in another
std::unique_ptr<ModelTraits> ReadFromFile(const std::string& filename);
void WriteToFile(const std::string& filename);
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

/**
 * Write a text representation of the ModelTraits to
 * the specified stream.
 * @param [in] model_traits the ModelTraits object to write to stream
 * @param [inout] the stream to write to. i.e. std::cout
 */
void WriteTextToStream(const ModelTraits *model_traits, std::ostream &stream);

} // namespace bc
#endif
