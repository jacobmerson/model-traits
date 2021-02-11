#ifndef BC_BC_NODE_H__
#define BC_BC_NODE_H__
#include "bcBC.h"
#include "bcGeometrySet.h"
#include "bcINode.h"
#include <memory>
#include <unordered_set>

namespace bc {
class BCNode : public INode {
  using geom_ptr_t = std::unique_ptr<IGeometrySet>;
  using bc_ptr_t = std::unique_ptr<IBoundaryCondition>;

public:
  virtual bool IsBoundaryCondition() const noexcept final { return true; }
  BCNode(std::string name, geom_ptr_t g, bc_ptr_t bc)
      : INode(std::move(name)), boundary_condition_(std::move(bc)),
        geometry_(std::move(g)) {}

  virtual void accept(NodeVisitor &v) final { v.visit(*this); }

private:
  std::unique_ptr<IBoundaryCondition> boundary_condition_;
  std::unique_ptr<IGeometrySet> geometry_;
};
} // namespace bc

#endif
