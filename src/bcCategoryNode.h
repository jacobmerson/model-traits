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

class CategoryNode : public INode, public ConvertFunctions<CategoryNode> {
public:
  CategoryNode(const std::string &name) : INode(name) {}
  CategoryNode(std::string &&name) : INode(std::move(name)) {}
  CategoryNode *AddCategory(std::string name) {
    return static_cast<CategoryNode *>(
        nodes_.AddNode(std::make_unique<CategoryNode>(std::move(name))));
  }

  /*
  BCNode *AddBoundaryCondition(std::string name, std::unique_ptr<IGeometrySet>
  geometry, std::unique_ptr<IBoundaryCondition> boundary_condition) { return
  static_cast<BCNode*>( nodes_.AddNode(std::make_unique<BCNode>(
            std::move(name), std::move(geometry),
            std::move(boundary_condition))));
  }
  */
  template <typename Geom, typename BC>
  BCNode *AddBoundaryCondition(std::string name, Geom &&geometry,
                               BC &&boundary_condition) {
    return static_cast<BCNode *>(nodes_.AddNode(std::make_unique<BCNode>(
        std::move(name), std::make_unique<Geom>(std::forward<Geom>(geometry)),
        std::make_unique<BC>(std::forward<BC>(boundary_condition)))));
  }

  virtual void accept(NodeVisitor &v) final { v.visit(*this); }

  friend fmt::formatter<CategoryNode>;

protected:
  NodeSet<> nodes_;
};
} // namespace bc
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
#endif
