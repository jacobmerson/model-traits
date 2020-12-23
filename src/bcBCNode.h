#ifndef BC_BC_NODE_H__
#define BC_BC_NODE_H__
#include "bcINode.h"
#include <unordered_set>

namespace bc {
template <typename GID, typename BC> class BCNode : public INode {
public:
  virtual bool IsBoundaryCondition() const final { return true; }
  BCNode(const std::string &name) : INode(name){};

private:
  BC boundary_condition_;
  // we may actually need to store the id and the dimension
  // need to verify how geometric id is typically reported
  std::unordered_set<GID> geometry_id_;
};
} // namespace bc

#endif
