#ifndef BC_BC_NODE_H__
#define BC_BC_NODE_H__
#include "Convert.h"
#include "GeometrySet.h"
#include "ModelTrait.h"
#include <memory>

namespace mt {
class ModelTraitNode : public Convertible<ModelTraitNode> {
public:
  using GeomPtrT = std::shared_ptr<IGeometrySet>;
  using MTPtrT = std::shared_ptr<IModelTrait>;
  ModelTraitNode() = default;
  ModelTraitNode(const std::string &name, GeomPtrT g, MTPtrT bc);
  void AddModelTrait(GeomPtrT g, MTPtrT bc);
  const std::vector<std::pair<GeomPtrT, MTPtrT>> &
  GetModelTraits() const noexcept;
  const std::string &GetName() const noexcept;

private:
  // each name can have multiple BC's associated with them as long as each
  // BC is classified on a unique set of geometric entities.
  std::vector<std::pair<GeomPtrT, MTPtrT>> model_traits_;
  std::string name_;
};
} // namespace mt

#endif
