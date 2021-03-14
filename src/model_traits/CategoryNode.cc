#include "model_traits/CategoryNode.h"

namespace mt {

CategoryNode::CategoryNode(const std::string &name) : name_(name) {}
CategoryNode::CategoryNode(std::string &&name) : name_(std::move(name)) {}
CategoryNode *CategoryNode::AddCategory(std::string name) {
  auto *nd = categories_.FindNode(name);
  if (nd != nullptr) {
    return nd;
  }
  return categories_.AddNode(std::make_unique<CategoryNode>(std::move(name)));
}
ModelTraitNode *
CategoryNode::AddModelTrait(const std::string &name,
                            std::shared_ptr<IGeometrySet> geometry,
                            std::shared_ptr<IModelTrait> model_trait) {
  auto nd = model_trait_nodes_.FindNode(name);
  // if the  model trait with a given name already exists
  if (nd) {
    nd->AddModelTrait(geometry, model_trait);
    return nd;
  } else {
    return model_trait_nodes_.AddNode(
        std::make_unique<ModelTraitNode>(name, geometry, model_trait));
  }
}
const CategoryNode::BCSetT &CategoryNode::GetModelTraitNodes() const {
  return model_trait_nodes_;
}
const CategoryNode::CategorySetT &CategoryNode::GetCategories() const {
  return categories_;
}
const std::string &CategoryNode::GetName() const noexcept { return name_; }
} // namespace mt