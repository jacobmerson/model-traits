#ifndef BC_CATEGORY_NODE_H__
#define BC_CATEGORY_NODE_H__
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
  CategoryNode(std::string name, std::string type = "");
  const CategoryNode *FindCategoryNode(const std::string &name) const noexcept;
  const ModelTraitNode *
  FindModelTraitNode(const std::string &name) const noexcept;
  CategoryNode *FindCategoryNode(const std::string &name) noexcept;
  ModelTraitNode *FindModelTraitNode(const std::string &name) noexcept;
  /**
   * Adds a category. If the internal memory grows, then it invalidates
   * the previously captured pointers
   */
  CategoryNode *AddCategory(std::string name, std::string type = "");
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
  ModelTraitNode *AddModelTrait(std::string name, std::vector<Geom> &geometry,
                                ModelTrait &&model_trait) {
    return AddModelTrait(
        std::move(name),
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
  ModelTraitNode *AddModelTrait(std::string name,
                                std::shared_ptr<IGeometrySet> geometry,
                                std::shared_ptr<IModelTrait> model_trait);

  const BCSetT &GetModelTraitNodes() const;
  const CategorySetT &GetCategoryNodes() const;
  const std::string &GetName() const noexcept;
  const std::string &GetType() const noexcept;

protected:
  // currently both categories, and model_traits are
  // stored with the vector backend. This is a reasonable assumption
  // with small numbers of nodes
  CategorySetT categories_;
  BCSetT model_trait_nodes_;
  std::string name_;
  std::string type_;
};
} // namespace mt
#endif
