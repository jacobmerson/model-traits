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
  // FIXME rename to AddOrFindCategory
  CategoryNode *AddCategory(std::string name) {
    auto nd = categories_.FindNode(name);
    if (nd) {
      return nd;
    }
    return categories_.AddNode(std::make_unique<CategoryNode>(std::move(name)));
  }

  BCNode *
  AddBoundaryCondition(const std::string &name,
                       std::shared_ptr<IGeometrySet> geometry,
                       std::shared_ptr<IBoundaryCondition> boundary_condition) {
    auto nd = boundary_conditions_.FindNode(name);
    // if the boundary condition with a given name already exists
    if (nd) {
      nd->AddBoundaryCondition(geometry, boundary_condition);
      return nd;
    } else {
      return boundary_conditions_.AddNode(
          std::make_unique<BCNode>(name, geometry, boundary_condition));
    }
  }

  template <typename Geom, typename BC>
  BCNode *AddBoundaryCondition(const std::string &name, Geom &&geometry,
                               BC &&boundary_condition) {

    return AddBoundaryCondition(
        name,
        std::static_pointer_cast<IGeometrySet>(
            std::make_shared<Geom>(std::forward<Geom>(geometry))),
        std::static_pointer_cast<IBoundaryCondition>(
            std::make_shared<BC>(std::forward<BC>(boundary_condition))));
  }

  // FIXME rename this? Not sure what to call it, but it's confusing
  // that it shares a name with GetBoundaryConditions in BCNode
  auto &GetBoundaryConditions() { return boundary_conditions_; }
  const auto &GetBoundaryConditions() const { return boundary_conditions_; }
  auto &GetCategories() { return categories_; }
  const auto &GetCategories() const { return categories_; }

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
