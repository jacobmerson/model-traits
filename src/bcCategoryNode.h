#ifndef BC_CATEGORY_NODE_H__
#define BC_CATEGORY_NODE_H__
#include "bcBC.h"
#include "bcBCNode.h"
#include "bcConvert.h"
#include "bcGeometrySet.h"
#include "bcINode.h"
#include "bcNodeSet.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace bc {

class CategoryNode : public INode, public Convertible<CategoryNode> {
  using NodeSetT = NodeSet<>;

public:
  CategoryNode(const std::string &name) : INode(name) {}
  CategoryNode(std::string &&name) : INode(std::move(name)) {}
  CategoryNode *AddCategory(std::string name) {
    return categories_.AddNode(std::make_unique<CategoryNode>(std::move(name)));
  }

  template <typename Geom, typename BC>
  BCNode *AddBoundaryCondition(std::string name, Geom &&geometry,
                               BC &&boundary_condition) {
    auto nd = boundary_conditions_.FindNode(name);
    // if the boundary condition with a given name already exists
    if (nd) {
      nd->AddBoundaryCondition(
          std::make_shared<Geom>(std::forward<Geom>(geometry)),
          std::make_shared<BC>(std::forward<BC>(boundary_condition)));
      return nd;
    } else {
      return boundary_conditions_.AddNode(std::make_unique<BCNode>(
          std::move(name), std::make_shared<Geom>(std::forward<Geom>(geometry)),
          std::make_shared<BC>(std::forward<BC>(boundary_condition))));
    }
  }

  // FIXME rename this? Not sure what to call it, but it's confusing
  // that it shares a name with GetBoundaryConditions in BCNode
  const auto &GetBoundaryConditions() { return boundary_conditions_; }
  const auto &GetCategories() { return categories_; }

  friend fmt::formatter<CategoryNode>;

protected:
  // currently both categories, and boundary_conditions are
  // stored with the vector backend. This is a reasonable assumption
  // with small numbers of nodes
  NodeSet<CategoryNode, BC_VEC_WORKAROUND> categories_;
  NodeSet<BCNode, BC_VEC_WORKAROUND> boundary_conditions_;
};
} // namespace bc
/*
template <> struct fmt::formatter<bc::CategoryNode> {
  int level = 0;
  int spaces = 2;
  constexpr auto parse(format_parse_context &ctx) {
    return parse_mt_nodes(ctx, level, spaces);
  };
  template <typename FormatContext>
  auto format(const bc::CategoryNode &cn, FormatContext &ctx) {
    auto out = format_to(ctx.out(), "{:>{}}{}", "-", level * spaces, cn.name_);
    for (auto &nd : cn.nodes_) {
      auto CN_ptr = dynamic_cast<bc::CategoryNode *>(nd.get());
      auto fmt_string = fmt::format("\n{{:{}.{}t}}", level + 1, spaces);
      if (CN_ptr) {
        out = format_to(out, fmt_string, *CN_ptr);
      } else {
        out = format_to(out, fmt_string, *nd);
      }
    }
    return out;
  }
};
*/
#endif
