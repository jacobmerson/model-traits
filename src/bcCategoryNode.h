#ifndef BC_CATEGORY_NODE_H__
#define BC_CATEGORY_NODE_H__
#include "bcBCNode.h"
#include "bcINode.h"
#include "bcNodeSet.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace bc {

class CategoryNode : public INode {
public:
  template <typename String,
            typename std::enable_if_t<
                std::is_convertible<String, std::string>::value, int> = 0>
  CategoryNode(String &&name) : INode(std::forward<String>(name)){};
  template <typename String,
            typename std::enable_if_t<
                std::is_convertible<String, std::string>::value, int> = 0>
  CategoryNode *AddCategory(String &&name) {
    return static_cast<CategoryNode *>(nodes_.AddNode(
        std::make_unique<CategoryNode>(std::forward<String>(name))));
  }
  template <typename GID, typename BC, typename GSet, typename String,
            typename std::enable_if_t<
                std::is_convertible<String, std::string>::value, int> = 0>
  BCNode<GID, BC> *AddBoundaryCondition(String &&name, GSet &&geometry,
                                        BC &&boundary_condition) {
    return static_cast<BCNode<GID, BC> *>(
        nodes_.AddNode(std::make_unique<BCNode<GID, BC>>(
            std::forward<String>(name), std::forward<GSet>(geometry),
            std::forward<BC>(boundary_condition))));
  }

protected:
  NodeSet<> nodes_;
};
} // namespace bc
#endif
