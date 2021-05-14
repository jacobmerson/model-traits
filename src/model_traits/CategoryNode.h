#ifndef MODEL_TRAITS_CATEGORY_NODE_H_
#define MODEL_TRAITS_CATEGORY_NODE_H_
#include "Convert.h"
#include "GeometrySet.h"
#include "ModelTrait.h"
#include "ModelTraitNode.h"
#include <memory>
#include <string>
#include <vector>

namespace mt {

class CategoryNode : public Convertible<CategoryNode> {
  using BCSetT = std::vector<ModelTraitNode>;
  using CategorySetT = std::vector<CategoryNode>;

public:
  CategoryNode() = default;
  CategoryNode(std::string type, std::string name);
  const ModelTraitNode *
  FindModelTraitNode(const std::string &name) const noexcept;
  ModelTraitNode *FindModelTraitNode(const std::string &name) noexcept;

  /**
   * Find the category node by the node type. If there are more than one
   * categories with the given type, you should use the FindCategoryNodesByType
   * function
   * @param type the node type to find
   * @return category node with the given type or nullptr
   */
  const CategoryNode *
  FindCategoryNodeByType(const std::string &type) const noexcept;
  CategoryNode *FindCategoryNodeByType(const std::string &type) noexcept;

  const CategoryNode *FindCategoryNode(const std::string &type,
                                       const std::string &name) const noexcept;
  CategoryNode *FindCategoryNode(const std::string &type,
                                 const std::string &name) noexcept;

  const CategoryNode *
  FindCategoryNodeByName(const std::string &name) const noexcept;
  CategoryNode *FindCategoryNodeByName(const std::string &name) noexcept;

  std::vector<const CategoryNode *>
  FindCategoryNodesByType(const std::string &type) const;
  std::vector<CategoryNode *> FindCategoryNodesByType(const std::string &type);
  /**
   * Adds a category. If the internal memory grows, then it invalidates
   * the previously captured pointers
   */
  CategoryNode *AddCategory(const std::string &type,
                            const std::string &name = "");
  /**
   * Adds a ModelTrait. If the internal memory grows, then it invalidates
   * the previously captured pointers
   */
  template <typename Geom, typename ModelTrait>
  ModelTraitNode *AddModelTrait(const std::string &name, Geom &&geometry,
                                ModelTrait &&model_trait) {

    return AddModelTrait(
        name,
        std::static_pointer_cast<IGeometrySet>(
            std::make_shared<Geom>(std::forward<Geom>(geometry))),
        std::static_pointer_cast<IModelTrait>(std::make_shared<ModelTrait>(
            std::forward<ModelTrait>(model_trait))));
  }
  /**
   * add model trait by passing a vector of geometry
   */
  template <typename Geom, typename ModelTrait>
  ModelTraitNode *AddModelTrait(const std::string &name,
                                const std::vector<Geom> &geometry,
                                ModelTrait &&model_trait) {
    return AddModelTrait(
        name,
        std::static_pointer_cast<IGeometrySet>(
            std::make_shared<GeometrySet<Geom>>(geometry.begin(),
                                                geometry.end())),
        std::static_pointer_cast<IModelTrait>(std::make_shared<ModelTrait>(
            std::forward<ModelTrait>(model_trait))));
  }
  template <typename Geom, typename ModelTrait>
  ModelTraitNode *AddModelTrait(const std::string &name,
                                std::vector<Geom> &geometry,
                                ModelTrait &&model_trait) {
    return AddModelTrait(
        name,
        std::static_pointer_cast<IGeometrySet>(
            std::make_shared<GeometrySet<Geom>>(geometry.begin(),
                                                geometry.end())),
        std::static_pointer_cast<IModelTrait>(std::make_shared<ModelTrait>(
            std::forward<ModelTrait>(model_trait))));
  }
  /**
   * Add the model trait by pointer.
   * If you are getting compiler errors when trying to use this overload make
   * sure that your types match exactly. If the types are not identical (have
   * consts,etc) you are probably accidentally calling the by value overload.
   */
  ModelTraitNode *AddModelTrait(const std::string &name,
                                std::shared_ptr<IGeometrySet> geometry,
                                std::shared_ptr<IModelTrait> model_trait);

  const BCSetT &GetModelTraitNodes() const;
  const CategorySetT &GetCategoryNodes() const;
  const std::string &GetType() const noexcept;
  const std::string &GetName() const noexcept;

protected:
  // currently both categories, and model_traits are
  // stored with the vector backend. This is a reasonable assumption
  // with small numbers of nodes
  CategorySetT categories_;
  BCSetT model_trait_nodes_;
  std::string type_;
  std::string name_;
};

/**
 * Gets the the first model trait node on the category with the given name
 * @param nd node to search for the category/model trait pair
 * @param type name of the category to get the model trait from
 * @return the first model trait on the category or nullptr if the category or
 * model trait don't exist
 */

const ModelTraitNode *
GetCategoryModelTraitNodeByType(const CategoryNode *nd,
                                const std::string &type) noexcept;
/**
 * Gets the the first model trait on the category with the given name
 * @param nd node to search for the category/model trait pair
 * @param type name of the category to get the model trait from
 * @return the first model trait on the category or nullptr if the category or
 * model trait don't exist
 */

const IModelTrait *
GetCategoryModelTraitByType(const CategoryNode *nd,
                            const std::string &type) noexcept;

template <typename ModelTrait>
const ModelTrait *
GetCategoryModelTraitByType(const CategoryNode *nd,
                            const std::string &type) noexcept {
  return MTCast<ModelTrait>(GetCategoryModelTraitByType(nd, type));
}
/**
 * Find the category node by type. If there is more than one
 * category with a given type, you will get any of the categories with the type.
 * You probably want to use GetCategoriesByType instead.
 * @param nd category node to search for the subcategory
 * @param name name of the category to find
 * @return category or nullptr if the category doesn't exist
 */
const CategoryNode *GetCategoryByType(const CategoryNode *nd,
                                      const std::string &type) noexcept;
/**
 * Get all of the categories with a given type
 * @param type the type of the category node
 * @return vector of pointers to category nodes with the given type
 */
const std::vector<const CategoryNode *>
GetCategoriesByType(const CategoryNode *, const std::string &type);
/**
 * Find the category node by name
 * @param nd category node to search for the subcategory
 * @param name name of the category to find
 * @return category or nullptr if the category doesn't exist
 */
const CategoryNode *GetCategoryByName(const CategoryNode *nd,
                                      const std::string &name) noexcept;

/**
 * gets the the first subcategory of a given category. Subcategories often carry
 * type information, so it's often the case that we want to get the value of the
 * first sub category
 * @param nd node to search
 * @param name category name
 * @return the value of the first subcategory of name or nullptr
 */
const CategoryNode *GetPrimaryCategoryByType(const CategoryNode *nd,
                                             const std::string &type) noexcept;

} // namespace mt
#endif
