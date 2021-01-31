#ifndef BC_CATEGORY_NODE_H__
#define BC_CATEGORY_NODE_H__
#include "bcINode.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace bc {

template <typename GID, typename BC> class BCNode;

class CategoryNode : public INode {
public:
  template <typename String,
            typename std::enable_if_t<
                std::is_convertible<String, std::string>::value, int> = 0>
  CategoryNode(String &&name) : INode(std::forward<String>(name)){};
  // TODO remove the add node interface
  // INode* AddNode(std::unique_ptr<INode> node);
  template <typename String,
            typename std::enable_if_t<
                std::is_convertible<String, std::string>::value, int> = 0>
  CategoryNode *AddCategory(String &&name);
  template <typename GID, typename BC, typename GSet, typename String,
            typename std::enable_if_t<
                std::is_convertible<String, std::string>::value, int> = 0>
  BCNode<GID, BC> *AddBoundaryCondition(String &&name, GSet &&geometry,
                                        BC &&boundary_condition);

protected:
  // note we need to constrain this to only allow
  // multiples if we are adding a BCNode
  std::unordered_map<std::string, std::vector<std::unique_ptr<INode>>> nodes_;
};
} // namespace bc
#include "bcCategoryNode_impl.h"
#endif
