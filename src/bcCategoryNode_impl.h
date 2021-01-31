#include "bcBCNode.h"
#include "fmt/format.h"
#include <exception>
namespace bc {
template <typename String,
          typename std::enable_if_t<
              std::is_convertible<String, std::string>::value, int> = 0>
CategoryNode *CategoryNode::AddCategory(String &&name) {
  auto it = nodes_.find(name);
  if (it != nodes_.end()) {
    if (it->second.back()->IsBoundaryCondition()) {
      throw std::runtime_error(fmt::format(
          "A BC node named {} already exists in the graph.",
          "boundary conditions and categories cannot share a name", name));
    }
    return static_cast<CategoryNode *>(it->second.back().get());
  } else {
    // don't forward the name here because we want to make a copy
    // we will forward/move the name when we add the case to the vector
    auto &nd = nodes_[name];
    nd.push_back(std::make_unique<CategoryNode>(std::forward<String>(name)));
    return static_cast<CategoryNode *>(nd.back().get());
  }
  return nullptr;
}
template <typename GID, typename BC, typename GSet, typename String,
          typename std::enable_if_t<
              std::is_convertible<String, std::string>::value, int> = 0>
BCNode<GID, BC> *CategoryNode::AddBoundaryCondition(String &&name,
                                                    GSet &&geometry,
                                                    BC &&boundary_condition) {
  auto it = nodes_.find(name);
  if (it != nodes_.end()) {
    if (!it->second.back()->IsBoundaryCondition()) {
      throw std::runtime_error(fmt::format(
          "Already assigned non BC node with name {}.",
          "Cannot add a bondary condition with the same name", name));
    } else {
      it->second.push_back(std::make_unique<BCNode<GID, BC>>(
          std::forward<String>(name), std::forward<GSet>(geometry),
          std::forward<BC>(boundary_condition)));
      return static_cast<BCNode<GID, BC> *>(it->second.back().get());
    }
  } else {
    auto &nd = nodes_[name];
    nd.push_back(std::make_unique<BCNode<GID, BC>>(
        std::forward<String>(name), std::forward<GSet>(geometry),
        std::forward<BC>(boundary_condition)));
    return static_cast<BCNode<GID, BC> *>(nd.back().get());
  }
}

} // namespace bc
