#ifndef BC_BC_NODE_H__
#define BC_BC_NODE_H__
#include "bcBC.h"
#include "bcConvert.h"
#include "bcGeometrySet.h"
#include "bcINode.h"
#include <memory>
#include <unordered_set>

namespace bc {
class BCNode : public INode, public Convertible<BCNode> {
  using GeomPtrT = std::shared_ptr<IGeometrySet>;
  using BCPtrT = std::shared_ptr<IBoundaryCondition>;

public:
  BCNode(const std::string &name, GeomPtrT g, BCPtrT bc) : INode(name) {
    bcs_.emplace_back(g, bc);
  }
  void AddBoundaryCondition(GeomPtrT g, BCPtrT bc) {
    bcs_.emplace_back(g, bc);
  };

  // FIXME rename this? It's a bit confusing that
  // it shares a name with GetBoundaryConditions in bcCategoryNode
  // which gets the boundary condition nodes
  const auto &GetBoundaryConditions() { return bcs_; }

private:
  // each name can have multiple BC's associated with them as long as each
  // BC is classified on a unique set of geometric entities.
  std::vector<std::pair<GeomPtrT, BCPtrT>> bcs_;
};
} // namespace bc

#endif
