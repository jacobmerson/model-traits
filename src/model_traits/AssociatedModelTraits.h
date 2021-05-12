#ifndef MODEL_TRAITS_CLASSIFIED_MODEL_TRAITS_H_
#define MODEL_TRAITS_CLASSIFIED_MODEL_TRAITS_H_
#include "CategoryNode.h"
#include "GeometrySet.h" // DimIdGeometry
#include "ModelTrait.h"
#include <unordered_map>
#include <vector>
namespace mt {

class AssociatedCategoryNode {
public:
  AssociatedCategoryNode() = default;
  AssociatedCategoryNode(std::string type, std::string name)
      : type_(std::move(type)), name_(std::move(name)) {}
  AssociatedCategoryNode *AddCategory(std::string type, std::string name);
  void AddModelTrait(const std::string &name,
                     const std::shared_ptr<const IModelTrait> &model_trait);

  const std::string &GetType() const noexcept;
  const std::string &GetName() const noexcept;
  const IModelTrait *FindModelTrait(const std::string &name) const noexcept;
  const AssociatedCategoryNode *
  FindCategoryByType(const std::string &type) const noexcept;
  const AssociatedCategoryNode *
  FindCategoryByName(const std::string &name) const noexcept;
  const AssociatedCategoryNode *
  FindCategory(const std::string &type, const std::string &name) const noexcept;
  std::vector<const AssociatedCategoryNode *>
  FindCategoriesByType(const std::string &type) const;

  /**
   * @return first model trait in the category
   */
  const IModelTrait *GetModelTrait() const noexcept;
  /**
   * convenience function which calls MTCast as well as GetModelTrait
   * @tparam ModelTrait the type that the model trait should be cased to
   * @return first model trait casted to ModelTrait* with MTCast
   */
  template <typename ModelTrait>
  const ModelTrait *GetModelTrait() const noexcept {
    return mt::MTCast<ModelTrait>(GetModelTrait());
  }
  std::vector<const IModelTrait *> GetModelTraits() const noexcept;
  std::size_t GetNumModelTraits() const noexcept;
  const std::vector<AssociatedCategoryNode> &GetCategories() const noexcept;
  std::size_t GetNumCategories() const noexcept;

private:
  AssociatedCategoryNode *FindCategory(const std::string &type,
                                       const std::string &name) noexcept;
  std::string type_;
  std::string name_;
  // important that category nodes are stored by value, so the deep
  // copy works as expected
  std::vector<AssociatedCategoryNode> categories_;
  // Since the same model trait may be referenced by multiple geometries,
  // it is stored as a shared ptr. It also shares ownership with the model
  // traits stored as part of ModelTraits
  std::unordered_map<std::string, std::shared_ptr<const IModelTrait>>
      model_traits_;
};

template <typename Geom>
class AssociatedGeometryNode : public AssociatedCategoryNode {
public:
  explicit AssociatedGeometryNode(const Geom &g)
      : AssociatedCategoryNode(), geom_(g) {}
  explicit AssociatedGeometryNode(Geom &&g)
      : AssociatedCategoryNode(), geom_(std::move(g)) {}
  const Geom &GetGeometry() const noexcept { return geom_; }

private:
  Geom geom_;
};

template <typename Geometry = DimIdGeometry> class AssociatedModelTraits {
public:
  AssociatedModelTraits() = default;
  /*
   * Constructs the associated model traits from a category node
   * The category node is most likely a top level case.
   */
  explicit AssociatedModelTraits(const CategoryNode *case_name);
  const std::vector<AssociatedGeometryNode<Geometry>> &
  GetGeometryNodes() const noexcept;
  std::size_t NumGeometricEntities() const noexcept;
  const AssociatedGeometryNode<Geometry> *
  Find(const Geometry &geometry) const noexcept;
  const AssociatedCategoryNode *GetNullGeometry() const noexcept;

  // copy construction and assignment are not allowed
  AssociatedModelTraits(const AssociatedModelTraits &) = delete;
  AssociatedModelTraits &operator=(const AssociatedModelTraits &) = delete;
  // move construction and assignment
  AssociatedModelTraits(AssociatedModelTraits &&) = default;
  AssociatedModelTraits &operator=(AssociatedModelTraits &&) = default;
  ~AssociatedModelTraits() = default;

private:
  std::vector<AssociatedGeometryNode<Geometry>> geometry_nodes_;
  // ptr because the value may, or may not exist
  std::unique_ptr<AssociatedCategoryNode> null_geometry_;
  void Process(const CategoryNode *current_node,
               std::vector<const CategoryNode *> &categories);
  void AddGeometry(const Geometry &geometry,
                   const std::vector<const CategoryNode *> &category_names,
                   const std::string &mt_name,
                   std::shared_ptr<const IModelTrait> model_trait);
  void AddNullGeometry(const std::vector<const CategoryNode *> &category_names,
                       const std::string &mt_name,
                       std::shared_ptr<const IModelTrait> model_trait);
};

/**
 * Factory function to create the associated model traits.
 */
template <typename Geom = DimIdGeometry>
std::unique_ptr<AssociatedModelTraits<Geom>>
AssociateModel(const CategoryNode *category_node) {
  return std::make_unique<AssociatedModelTraits<Geom>>(category_node);
}
/**
 * Gets the the first model trait on the category with the given name
 * @param nd node to search for the category/model trait pair
 * @param type type of the category to get the model trait from
 * @return the first model trait on the category or nullptr if the category or
 * model trait don't exist
 */

const IModelTrait *
GetCategoryModelTraitByType(const AssociatedCategoryNode *nd,
                            const std::string &type) noexcept;
/**
 *
 * @tparam ModelTrait type to cast the model trait to
 * @param nd node to search for the category/model trait pair
 * @param type the type of the category to get the model trait from
 * @return the first model trait on the category or nullptr if the category or
 * model trait don't exist
 */
 template <typename ModelTrait>
 const ModelTrait *
 GetCategoryModelTraitByType(const AssociatedCategoryNode *nd,
                                        const std::string &type) noexcept {
  return MTCast<ModelTrait>(GetCategoryModelTraitByType(nd, type));
}
/**
 *
 * @param nd category node to search for the subcategory
 * @param type type of the category to find
 * @return category or nullptr if the category doesn't exist
 */
const AssociatedCategoryNode *
GetCategoryByType(const AssociatedCategoryNode *nd,
                  const std::string &type) noexcept;

const AssociatedCategoryNode *
GetCategoryByName(const AssociatedCategoryNode *nd,
                  const std::string &name) noexcept;
const AssociatedCategoryNode *GetCategory(const AssociatedCategoryNode *nd,
                                          const std::string &type,
                                          const std::string &name) noexcept;
std::vector<const AssociatedCategoryNode *>
GetCategoriesByType(const AssociatedCategoryNode *nd, const std::string &type);
/**
 * gets the the first subcategory of a given category. Subcategories often
 * carry type information, so it's often the case that we want to get the
 * value of the first sub category
 * @param nd node to search
 * @param name category name
 * @return the value of the first subcategory of name or nullptr
 */
const AssociatedCategoryNode *
GetPrimaryCategoryByType(const AssociatedCategoryNode *nd,
                         const std::string &type) noexcept;

} // namespace mt

#endif
