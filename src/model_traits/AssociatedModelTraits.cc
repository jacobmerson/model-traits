#include "AssociatedModelTraits.h"
#include <algorithm>
#include <fmt/format.h>

namespace mt {

template <typename Geom>
void AssociatedModelTraits<Geom>::AddGeometry(
    const Geom &geom, const std::vector<const CategoryNode *> &categories,
    const std::string &mt_name, std::shared_ptr<const IModelTrait> mt) {
  using std::find_if;
  // auto it = find(begin(geometry_),end(geometry_), g);
  auto it = find_if(begin(geometry_nodes_), end(geometry_nodes_),
                    [&geom](const AssociatedGeometryNode<Geom> &node) {
                      return node.GetGeometry() == geom;
                    });
  AssociatedCategoryNode *node = nullptr;
  if (it == geometry_nodes_.end()) {
    geometry_nodes_.emplace_back(geom);
    node = &geometry_nodes_.back();
  } else {
    node = &(*it);
  }
  for (const auto *category : categories) {
    node = node->AddCategory(category->GetType(), category->GetName());
  }
  node->AddModelTrait(mt_name, std::move(mt));
}

template <typename Geom>
void AssociatedModelTraits<Geom>::Process(
    const CategoryNode *current_node,
    std::vector<const CategoryNode *> &categories) {
  // At a leaf node, merge the contents into the
  for (const auto &bc_node : current_node->GetModelTraitNodes()) {
    for (const auto &bc : bc_node.GetModelTraits()) {
      auto boundary_condition = bc.second;
      // only add the geometry type of interest
      auto geom_set = std::dynamic_pointer_cast<GeometrySet<Geom>>(bc.first);
      if (geom_set) {
        // the set is empty, so add the null geometry
        if (geom_set->begin() == geom_set->end()) {
          AddNullGeometry(categories, bc_node.GetName(), boundary_condition);
        } else {
          for (auto &g : *geom_set) {
            AddGeometry(g, categories, bc_node.GetName(), boundary_condition);
          }
        }
      }
    }
  }
  for (const auto &category : current_node->GetCategoryNodes()) {
    // copy associated nodes, on each call it only holds one DFS arm of the tree
    auto categories_copy = categories;
    categories_copy.push_back(&category);
    Process(&category, categories_copy);
  }
}

template <typename Geom>
AssociatedModelTraits<Geom>::AssociatedModelTraits(
    const CategoryNode *root_category_node) {
  std::vector<const CategoryNode *> categories;
  Process(root_category_node, categories);
}
template <typename Geometry>
const std::vector<AssociatedGeometryNode<Geometry>> &
AssociatedModelTraits<Geometry>::GetGeometryNodes() const noexcept {
  return geometry_nodes_;
}
template <typename Geometry>
std::size_t
AssociatedModelTraits<Geometry>::NumGeometricEntities() const noexcept {
  return geometry_nodes_.size() + (null_geometry_ != nullptr);
}
template <typename Geometry>
const AssociatedGeometryNode<Geometry> *
AssociatedModelTraits<Geometry>::Find(const Geometry &geometry) const noexcept {
  using std::find_if;
  auto it = find_if(begin(geometry_nodes_), end(geometry_nodes_),
                    [&geometry](const AssociatedGeometryNode<Geometry> &node) {
                      return geometry == node.GetGeometry();
                    });
  if (it == end(geometry_nodes_)) {
    return nullptr;
  }
  return &(*it);
}
template <typename Geometry>
const AssociatedCategoryNode *
AssociatedModelTraits<Geometry>::GetNullGeometry() const noexcept {
  return null_geometry_.get();
}
template <typename Geometry>
void AssociatedModelTraits<Geometry>::AddNullGeometry(
    const std::vector<const CategoryNode *> &categories,
    const std::string &mt_name,
    std::shared_ptr<const IModelTrait> model_trait) {
  if (null_geometry_ == nullptr) {
    null_geometry_ = std::make_unique<AssociatedCategoryNode>();
  }
  AssociatedCategoryNode *node{null_geometry_.get()};
  for (auto *category : categories) {
    node = node->AddCategory(category->GetType(), category->GetName());
  }
  node->AddModelTrait(mt_name, std::move(model_trait));
}
AssociatedCategoryNode *AssociatedCategoryNode::AddCategory(std::string type,
                                                            std::string name) {
  auto *cat = FindCategory(type, name);
  if (cat == nullptr) {
    categories_.emplace_back(std::move(type), std::move(name));
    return &categories_.back();
  }
  return cat;
}
void AssociatedCategoryNode::AddModelTrait(
    const std::string &name,
    const std::shared_ptr<const IModelTrait> &model_trait) {
  auto ret = model_traits_.insert(std::make_pair(name, model_trait));
  if (!ret.second) {
    throw std::runtime_error(
        fmt::format("Cannot add two boundary conditions with "
                    "the name {} to a given geometry/category.",
                    name));
  }
}
const std::string &AssociatedCategoryNode::GetType() const noexcept {
  return type_;
}
const std::string &AssociatedCategoryNode::GetName() const noexcept {
  return name_;
}
const IModelTrait *
AssociatedCategoryNode::FindModelTrait(const std::string &name) const noexcept {
  auto it = model_traits_.find(name);
  if (it == end(model_traits_)) {
    return nullptr;
  }
  return it->second.get();
}
const AssociatedCategoryNode *AssociatedCategoryNode::FindCategoryByType(
    const std::string &type) const noexcept {
  auto it = std::find_if(begin(categories_), end(categories_),
                         [&type](const AssociatedCategoryNode &node) {
                           return type == node.GetType();
                         });
  if (it == end(categories_)) {
    return nullptr;
  }
  return &(*it);
}
const IModelTrait *AssociatedCategoryNode::GetModelTrait() const noexcept {
  if (model_traits_.size() == 0) {
    return nullptr;
  }
  return model_traits_.begin()->second.get();
}
std::vector<const IModelTrait *>
AssociatedCategoryNode::GetModelTraits() const noexcept {
  std::vector<const IModelTrait *> traits;
  traits.reserve(model_traits_.size());
  for (const auto &trait : model_traits_) {
    traits.push_back(trait.second.get());
  }
  return traits;
}
std::size_t AssociatedCategoryNode::GetNumModelTraits() const noexcept {
  return model_traits_.size();
}
const std::vector<AssociatedCategoryNode> &
AssociatedCategoryNode::GetCategories() const noexcept {
  return categories_;
}
std::size_t AssociatedCategoryNode::GetNumCategories() const noexcept {
  return categories_.size();
}
const AssociatedCategoryNode *AssociatedCategoryNode::FindCategoryByName(
    const std::string &name) const noexcept {
  auto it = std::find_if(begin(categories_), end(categories_),
                         [&name](const AssociatedCategoryNode &node) {
                           return name == node.GetName();
                         });
  if (it == end(categories_)) {
    return nullptr;
  }
  return &(*it);
}
const AssociatedCategoryNode *
AssociatedCategoryNode::FindCategory(const std::string &type,
                                     const std::string &name) const noexcept {
  auto it =
      std::find_if(begin(categories_), end(categories_),
                   [&name, &type](const AssociatedCategoryNode &node) {
                     return name == node.GetName() && type == node.GetType();
                   });
  if (it == end(categories_)) {
    return nullptr;
  }
  return &(*it);
}
AssociatedCategoryNode *
AssociatedCategoryNode::FindCategory(const std::string &type,
                                     const std::string &name) noexcept {
  return const_cast<AssociatedCategoryNode *>(
      static_cast<const AssociatedCategoryNode &>(*this).FindCategory(type,
                                                                      name));
}
std::vector<const AssociatedCategoryNode *>
AssociatedCategoryNode::FindCategoriesByType(const std::string &type) const {
  std::vector<const AssociatedCategoryNode *> nodes;
  for (const auto &category : categories_) {
    if (category.GetType() == type) {
      nodes.push_back(&category);
    }
  }
  return nodes;
}
const IModelTrait *
GetCategoryModelTraitByType(const AssociatedCategoryNode *nd,
                            const std::string &name) noexcept {
  if (nd == nullptr) {
    return nullptr;
  }
  const auto *category = nd->FindCategoryByType(name);
  if (category == nullptr) {
    return nullptr;
  }
  return category->GetModelTrait();
}
const AssociatedCategoryNode *
GetCategoryByType(const AssociatedCategoryNode *nd,
                  const std::string &type) noexcept {
  if (nd == nullptr) {
    return nullptr;
  }
  return nd->FindCategoryByType(type);
}
const AssociatedCategoryNode *
GetPrimaryCategoryByType(const AssociatedCategoryNode *nd,
                         const std::string &type) noexcept {
  const auto *category = GetCategoryByType(nd, type);
  if (category == nullptr) {
    return nullptr;
  }
  const auto &subcategories = category->GetCategories();
  if (subcategories.size() == 0) {
    return nullptr;
  }
  return &subcategories.front();
}
const AssociatedCategoryNode *
GetCategoryByName(const AssociatedCategoryNode *nd,
                  const std::string &name) noexcept {
  if (nd == nullptr) {
    return nullptr;
  }
  return nd->FindCategoryByName(name);
}
const AssociatedCategoryNode *GetCategory(const AssociatedCategoryNode *nd,
                                          const std::string &type,
                                          const std::string &name) noexcept {
  if (nd == nullptr) {
    return nullptr;
  }
  return nd->FindCategory(type, name);
}
std::vector<const AssociatedCategoryNode *>
GetCategoriesByType(const AssociatedCategoryNode *nd, const std::string &type) {
  if (nd == nullptr) {
    return {};
  }
  return nd->FindCategoriesByType(type);
}

template class AssociatedModelTraits<DimIdGeometry>;
template class AssociatedModelTraits<IdGeometry>;
} // namespace mt
