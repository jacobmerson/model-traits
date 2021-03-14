#ifndef BC_BC_NODE_H__
#define BC_BC_NODE_H__
#include "Convert.h"
#include "GeometrySet.h"
#include "INode.h"
#include "ModelTrait.h"
#include <memory>

namespace mt {
class ModelTraitNode : public INode, public Convertible<ModelTraitNode> {
public:
  using GeomPtrT = std::shared_ptr<IGeometrySet>;
  using MTPtrT = std::shared_ptr<IModelTrait>;
  ModelTraitNode(const std::string &name, GeomPtrT g, MTPtrT bc) : INode(name) {
    model_traits_.emplace_back(std::move(g), std::move(bc));
  }
  void AddModelTrait(GeomPtrT g, MTPtrT bc) {
    model_traits_.emplace_back(std::move(g), std::move(bc));
  };

  std::vector<std::pair<GeomPtrT, MTPtrT>> &GetModelTraits() noexcept {
    return model_traits_;
  }
  const std::vector<std::pair<GeomPtrT, MTPtrT>> &
  GetModelTraits() const noexcept {
    return model_traits_;
  }

private:
  // each name can have multiple BC's associated with them as long as each
  // BC is classified on a unique set of geometric entities.
  std::vector<std::pair<GeomPtrT, MTPtrT>> model_traits_;
};
} // namespace mt

#endif
