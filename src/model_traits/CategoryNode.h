#ifndef BC_CATEGORY_NODE_H__
#define BC_CATEGORY_NODE_H__
#include "Convert.h"
#include "GeometrySet.h"
#include "ModelTrait.h"
#include "ModelTraitNode.h"
#include "NodeSet.h"
#include <memory>
#include <string>
#include <vector>

namespace mt {

class CategoryNode : public Convertible<CategoryNode> {
  using BCSetT = NodeSet<ModelTraitNode, BC_VEC_WORKAROUND>;
  using CategorySetT = NodeSet<CategoryNode, BC_VEC_WORKAROUND>;

public:
  explicit CategoryNode(const std::string &name);
  explicit CategoryNode(std::string &&name);
  CategoryNode *AddCategory(std::string name);

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

  const BCSetT &GetModelTraitNodes() const;
  const CategorySetT &GetCategories() const;
  const std::string &GetName() const noexcept;

  friend fmt::formatter<CategoryNode>;

protected:
  ModelTraitNode *AddModelTrait(const std::string &name,
                                std::shared_ptr<IGeometrySet> geometry,
                                std::shared_ptr<IModelTrait> model_trait);
  // currently both categories, and model_traits are
  // stored with the vector backend. This is a reasonable assumption
  // with small numbers of nodes
  CategorySetT categories_;
  BCSetT model_trait_nodes_;
  std::string name_;
};
} // namespace mt
#endif
