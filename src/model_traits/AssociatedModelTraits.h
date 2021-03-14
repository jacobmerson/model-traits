#ifndef BC_CLASSIFIED_MODEL_TRAITS_H__
#define BC_CLASSIFIED_MODEL_TRAITS_H__
#include "CategoryNode.h"
#include "GeometrySet.h" // DimGeometry
#include "ModelTrait.h"
#include <unordered_map>
#include <vector>
namespace mt {
class AssociatedCategoryNode {
public:
  explicit AssociatedCategoryNode(std::string name) : name_(std::move(name)) {}
  AssociatedCategoryNode *AddCategory(std::string name);
  void AddModelTrait(const std::string &name,
                     const std::shared_ptr<const IModelTrait> &model_trait);

  AssociatedCategoryNode(const AssociatedCategoryNode &) noexcept = default;
  AssociatedCategoryNode &
  operator=(const AssociatedCategoryNode &) noexcept = default;
  AssociatedCategoryNode(AssociatedCategoryNode &&) noexcept = default;
  AssociatedCategoryNode &
  operator=(AssociatedCategoryNode &&) noexcept = default;
  virtual ~AssociatedCategoryNode() noexcept = default;
  const std::string &GetName() const noexcept;
  const IModelTrait *FindModelTrait(const std::string &name) const;
  const AssociatedCategoryNode *FindCategory(const std::string &name) const;

private:
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
      : AssociatedCategoryNode(""), geom_(g) {}
  explicit AssociatedGeometryNode(Geom &&g)
      : AssociatedCategoryNode(""), geom_(std::move(g)) {}
  const Geom &GetGeometry() const noexcept { return geom_; }

private:
  Geom geom_;
};

template <typename Geometry> class AssociatedModelTraits {
public:
  /*
   * Constructs the associated model traits from a category node
   * The category node is most likely a top level case.
   */
  explicit AssociatedModelTraits(const CategoryNode *);
  const std::vector<AssociatedGeometryNode<Geometry>> &
  GetGeometryNodes() const noexcept;
  std::size_t NumGeometricEntities() const noexcept;
  const AssociatedGeometryNode<Geometry> *Find(const Geometry &geometry);

private:
  // IAssociatedGeometryNode *
  // AddGeometryNode(std::unique_ptr<IAssociatedGeometryNode> geom);
  std::vector<AssociatedGeometryNode<Geometry>> geometry_nodes_;
  void Process(const CategoryNode *current_node,
               std::vector<std::string> &categories);
  void AddGeometry(const Geometry &, const std::vector<std::string> &,
                   const std::string &, std::shared_ptr<const IModelTrait>);
};

} // namespace mt

#endif
