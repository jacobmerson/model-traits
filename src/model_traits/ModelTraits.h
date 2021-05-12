#ifndef MODEL_TRAITS_MODEL_TRAITS_H_
#define MODEL_TRAITS_MODEL_TRAITS_H_
#include "CategoryNode.h"
#include <memory>
#include <string>

namespace mt {
/**
 * ModelTraits object is the root object that stores any boundary conditions, or
 * other attributes that may be stored on a model.
 */
class ModelTraits : public Convertible<ModelTraits> {
public:
  /**
   * @param [in] name of the root model traits object
   */
  explicit ModelTraits(const std::string &name);
  /**
   * @param [in] name the root model traits object
   */
  explicit ModelTraits(std::string &&name);
  /**
   * @brief Add case to model traits object
   * @param [in] name
   * @return pointer to the inserted value if it wasn't already in the map or
   * nullptr
   *
   * Adds a case if it doesn't exist and returns a pointer to it's
   * INode. If the case does exist, the pointer to the case is returned.
   */
  CategoryNode *AddCase(const std::string &name);
  const CategoryNode *FindCase(const std::string &name) const noexcept;
  CategoryNode *FindCase(const std::string &name) noexcept;
  /**
   * Removes a case from ModelTraits by it's name
   */
  void RemoveCase(const std::string &name);
  const std::vector<CategoryNode> &GetCases() const;
  int NumCases();
  const std::string &GetName() const noexcept;

protected:
  std::vector<CategoryNode> cases_;
  std::string name_;
};

} // namespace mt

#endif
