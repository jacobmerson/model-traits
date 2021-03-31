#include "model_traits/CategoryNode.h"
#include <algorithm>
#include <string>

namespace mt {

using std::find_if;

CategoryNode::CategoryNode(std::string type, std::string name)
    : type_(std::move(type)), name_(std::move(name)) {}
CategoryNode *CategoryNode::AddCategory(const std::string &type,
                                        const std::string &name) {
  CategoryNode *nd = FindCategoryNode(type, name);
  if (nd == nullptr) {
    categories_.emplace_back(type, name);
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
const std::string &CategoryNode::GetType() const noexcept { return type_; }
const std::string &CategoryNode::GetName() const noexcept { return name_; }

const CategoryNode *
CategoryNode::FindCategoryNodeByType(const std::string &type) const noexcept {
  auto it = find_if(
      begin(categories_), end(categories_),
      [&type](const CategoryNode &node) { return type == node.GetType(); });
  if (it == end(categories_)) {
    return nullptr;
  }
  return &(*it);
}
const ModelTraitNode *
CategoryNode::FindModelTraitNode(const std::string &type) const noexcept {
  auto it = find_if(
      model_trait_nodes_.begin(), model_trait_nodes_.end(),
      [&type](const ModelTraitNode &node) { return node.GetName() == type; });
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
CategoryNode *
CategoryNode::FindCategoryNodeByType(const std::string &type) noexcept {
  return const_cast<CategoryNode *>(
      static_cast<const CategoryNode &>(*this).FindCategoryNodeByType(type));
}
const CategoryNode *
CategoryNode::FindCategoryNode(const std::string &type,
                               const std::string &name) const noexcept {
  auto it =
      find_if(begin(categories_), end(categories_),
              [&type, &name](const CategoryNode &node) {
                return (type == node.GetType()) && (name == node.GetName());
              });
  if (it == end(categories_)) {
    return nullptr;
  }
  return &(*it);
}
CategoryNode *CategoryNode::FindCategoryNode(const std::string &type,
                                             const std::string &name) noexcept {
  return const_cast<CategoryNode *>(
      static_cast<const CategoryNode &>(*this).FindCategoryNode(type, name));
}
const CategoryNode *
CategoryNode::FindCategoryNodeByName(const std::string &name) const noexcept {
  auto it = find_if(
      begin(categories_), end(categories_),
      [&name](const CategoryNode &node) { return name == node.GetName(); });
  if (it == end(categories_)) {
    return nullptr;
  }
  return &(*it);
}
CategoryNode *
CategoryNode::FindCategoryNodeByName(const std::string &name) noexcept {
  return const_cast<CategoryNode *>(
      static_cast<const CategoryNode &>(*this).FindCategoryNodeByName(name));
}
std::vector<const CategoryNode *>
CategoryNode::FindCategoryNodesByType(const std::string &type) const {
  std::vector<const CategoryNode *> nodes;
  for (const auto &cat : categories_) {
    if (cat.GetType() == type) {
      nodes.push_back(&cat);
    }
  }
  return nodes;
}
std::vector<CategoryNode *>
CategoryNode::FindCategoryNodesByType(const std::string &type) {
  std::vector<CategoryNode *> nodes;
  for (auto &cat : categories_) {
    if (cat.GetType() == type) {
      nodes.push_back(&cat);
    }
  }
  return nodes;
}

const ModelTraitNode *
GetCategoryModelTraitNode(const CategoryNode *nd,
                          const std::string &name) noexcept {
  if (nd == nullptr) {
    return nullptr;
  }
  const auto *category = nd->FindCategoryNodeByType(name);
  if (category == nullptr) {
    return nullptr;
  }
  return &category->GetModelTraitNodes().front();
}

const CategoryNode *GetCategoryByType(const CategoryNode *nd,
                                      const std::string &type) noexcept {
  if (nd == nullptr) {
    return nullptr;
  }
  return nd->FindCategoryNodeByType(type);
}
const CategoryNode *GetPrimaryCategoryByType(const CategoryNode *nd,
                                             const std::string &type) noexcept {
  const auto *category = GetCategoryByType(nd, type);
  if (category == nullptr) {
    return nullptr;
  }
  const auto &subcategories = category->GetCategoryNodes();
  if (subcategories.size() == 0) {
    return nullptr;
  }
  return &subcategories.front();
}
const std::vector<const CategoryNode *>
GetCategoriesByType(const CategoryNode *nd, const std::string &type) {
  if (nd == nullptr) {
    return {};
  }
  return nd->FindCategoryNodesByType(type);
}
const CategoryNode *GetCategoryByName(const CategoryNode *nd,
                                      const std::string &name) noexcept {
  if (nd == nullptr) {
    return nullptr;
  }
  return nd->FindCategoryNodeByName(name);
}
} // namespace mt