#include "model_traits/CategoryNode.h"
#include <algorithm>
#include <fmt/format.h>

namespace mt {

using std::find_if;

CategoryNode::CategoryNode(const std::string &name) : name_(name) {}
CategoryNode::CategoryNode(std::string &&name) : name_(std::move(name)) {}
CategoryNode *CategoryNode::AddCategory(const std::string &name) {
  CategoryNode *nd = FindCategoryNode(name);
  if (nd == nullptr) {
    categories_.emplace_back(std::move(name));
    return &categories_.back();
  }
  return nd;
}
ModelTraitNode *
CategoryNode::AddModelTrait(const std::string &name,
                            std::shared_ptr<IGeometrySet> geometry,
                            std::shared_ptr<IModelTrait> model_trait) {
  ModelTraitNode *nd = FindModelTraitNode(name);
  if (nd == nullptr) {
    model_trait_nodes_.emplace_back(name, geometry, model_trait);
    return &(model_trait_nodes_.back());
  }
  nd->AddModelTrait(geometry, model_trait);
  return nd;
}

const CategoryNode::BCSetT &CategoryNode::GetModelTraitNodes() const {
  return model_trait_nodes_;
}
const CategoryNode::CategorySetT &CategoryNode::GetCategoryNodes() const {
  return categories_;
}
const std::string &CategoryNode::GetName() const noexcept { return name_; }

const CategoryNode *
CategoryNode::FindCategoryNode(const std::string &name) const noexcept {
  auto it = find_if(
      begin(categories_), end(categories_),
      [&name](const CategoryNode &node) { return name == node.GetName(); });
  if (it == end(categories_)) {
    return nullptr;
  }
  return &(*it);
}
const ModelTraitNode *
CategoryNode::FindModelTraitNode(const std::string &name) const noexcept {
  auto it = find_if(
      model_trait_nodes_.begin(), model_trait_nodes_.end(),
      [&name](const ModelTraitNode &node) { return node.GetName() == name; });
  if (it == end(model_trait_nodes_)) {
    return nullptr;
  }
  return &(*it);
}
ModelTraitNode *
CategoryNode::FindModelTraitNode(const std::string &name) noexcept {
  return const_cast<ModelTraitNode *>(
      static_cast<const CategoryNode &>(*this).FindModelTraitNode(name));
}
CategoryNode *CategoryNode::FindCategoryNode(const std::string &name) noexcept {
  return const_cast<CategoryNode *>(
      static_cast<const CategoryNode &>(*this).FindCategoryNode(name));
}
} // namespace mt