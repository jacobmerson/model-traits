#ifndef BC_CATEGORY_NODE_H__
#define BC_CATEGORY_NODE_H__
#include "bcINode.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace bc {
// Should this be called a CNode? I feel like
// having things spelled out makes it slightly
// more clear
class CategoryNode : public INode {
public:
  CategoryNode(const std::string &name) : INode(name){};
  void AddNode(std::unique_ptr<INode> node);

protected:
  // note we need to constrain this to only allow
  // multiples if we are adding a BCNode
  std::unordered_map<std::string, std::vector<std::unique_ptr<INode>>> nodes_;
};
} // namespace bc
#endif
