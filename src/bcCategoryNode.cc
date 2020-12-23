#include "bcCategoryNode.h"
#include "fmt/format.h"
#include <cassert>
#include <exception>

namespace bc {
void CategoryNode::AddNode(std::unique_ptr<INode> node) {
  auto it = nodes_.find(node->GetName());
  if (it != nodes_.end()) {
    // if the name is already in the map the vector
    // must always have at least one node stored
    assert(it->second.size() > 0);
    if (!it->second[0]->IsBoundaryCondition()) {
      throw std::runtime_error(fmt::format(
          "already assigned non BC node with name {}. Cannot add another node",
          node->GetName()));
    } else if (node->IsBoundaryCondition()) {
      it->second.push_back(std::move(node));
    } else {
      throw std::runtime_error(
          fmt::format("Cannot add non-BC node with name {} to BC node list",
                      node->GetName()));
    }
  } else {
    auto name = node->GetName();
    std::vector<std::unique_ptr<INode>> vec{};
    vec.push_back(std::move(node));
    nodes_.insert(std::make_pair(name, std::move(vec)));
  }
}
} // namespace bc
