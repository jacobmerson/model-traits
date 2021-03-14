#include "bcAssociatedModelTraits.h"
#include <algorithm>
#include <fmt/format.h>

namespace mt {

template <typename Geom>
void AssociatedModelTraits<Geom>::AddGeometry(
    const Geom &geom, const std::vector<std::string> &category_names,
    const std::string &bc_name, std::shared_ptr<const IBoundaryCondition> bc) {
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
  for (const auto &category_name : category_names) {
    node = node->AddCategory(category_name);
  }
  node->AddBoundaryCondition(bc_name, std::move(bc));
}

template <typename Geom>
void AssociatedModelTraits<Geom>::Process(
    const CategoryNode *current_node, std::vector<std::string> &categories) {
  // At a leaf node, merge the contents into the
  for (const auto &bc_node : current_node->GetBoundaryConditionNodes()) {
    for (const auto &bc : bc_node->GetBoundaryConditions()) {
      auto boundary_condition = bc.second;
      // only add the geometry type of interest
      auto geom_set = std::dynamic_pointer_cast<GeometrySet<Geom>>(bc.first);
      if (geom_set) {
        for (auto &g : *geom_set) {
          AddGeometry(g, categories, bc_node->GetName(), boundary_condition);
        }
      }
    }
  }
  for (const auto &category : current_node->GetCategories()) {
    // copy associated nodes, on each call it only holds one DFS arm of the tree
    auto categories_copy = categories;
    categories_copy.push_back(category->GetName());
    Process(category.get(), categories_copy);
  }
}

template <typename Geom>
AssociatedModelTraits<Geom>::AssociatedModelTraits(
    const CategoryNode *root_category_node) {
  std::vector<std::string> categories;
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
  return geometry_nodes_.size();
}
template <typename Geometry>
const AssociatedGeometryNode<Geometry> *
AssociatedModelTraits<Geometry>::Find(const Geometry &geometry) {
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
AssociatedCategoryNode *AssociatedCategoryNode::AddCategory(std::string name) {
  using std::find_if;
  auto it = find_if(begin(categories_), end(categories_),
                    [&name](const AssociatedCategoryNode &nd) {
                      return nd.GetName() == name;
                    });
  if (it == end(categories_)) {
    categories_.emplace_back(std::move(name));
    return &categories_.back();
  }
  return &(*it);
}
void AssociatedCategoryNode::AddBoundaryCondition(
    const std::string &name,
    const std::shared_ptr<const IBoundaryCondition> &bc) {
  auto ret = boundary_conditions_.insert(std::make_pair(name, bc));
  if (!ret.second) {
    throw std::runtime_error(
        fmt::format("Cannot add two boundary conditions with "
                    "the name {} to a given geometry/category.",
                    name));
  }
}
const std::string &AssociatedCategoryNode::GetName() const noexcept {
  return name_;
}
const IBoundaryCondition *
AssociatedCategoryNode::FindBoundaryCondition(const std::string &name) const {
  auto it = boundary_conditions_.find(name);
  if (it == end(boundary_conditions_)) {
    return nullptr;
  }
  return it->second.get();
}
const AssociatedCategoryNode *
AssociatedCategoryNode::FindCategory(const std::string &name) const {
  using std::find_if;
  auto it = find_if(begin(categories_), end(categories_),
                    [&name](const AssociatedCategoryNode &node) {
                      return name == node.GetName();
                    });
  if (it == end(categories_)) {
    return nullptr;
  }
  return &(*it);
}

template class AssociatedModelTraits<DimGeometry>;
template class AssociatedModelTraits<OrdinalType>;
} // namespace bc