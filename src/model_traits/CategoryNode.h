#ifndef BC_CATEGORY_NODE_H__
#define BC_CATEGORY_NODE_H__
#include "Convert.h"
#include "GeometrySet.h"
#include "ModelTrait.h"
#include "ModelTraitNode.h"
#include "NodeSet.h"
#include <memory>
#include <string>
#include <vector>

namespace mt {

class CategoryNode : public Convertible<CategoryNode> {
  using BCSetT = NodeSet<ModelTraitNode, BC_VEC_WORKAROUND>;
  using CategorySetT = NodeSet<CategoryNode, BC_VEC_WORKAROUND>;

public:
  explicit CategoryNode(const std::string &name) : name_(name) {}
  explicit CategoryNode(std::string &&name) : name_(std::move(name)) {}
  // FIXME rename to AddOrFindCategory
  CategoryNode *AddCategory(std::string name) {
    auto *nd = categories_.FindNode(name);
    if (nd != nullptr) {
      return nd;
    }
    return categories_.AddNode(std::make_unique<CategoryNode>(std::move(name)));
  }

  ModelTraitNode *AddModelTrait(const std::string &name,
                                std::shared_ptr<IGeometrySet> geometry,
                                std::shared_ptr<IModelTrait> model_trait) {
    auto nd = model_trait_nodes_.FindNode(name);
    // if the  model trait with a given name already exists
    if (nd) {
      nd->AddModelTrait(geometry, model_trait);
      return nd;
    } else {
      return model_trait_nodes_.AddNode(
          std::make_unique<ModelTraitNode>(name, geometry, model_trait));
    }
  }

  template <typename Geom, typename ModelTrait>
  ModelTraitNode *AddModelTrait(const std::string &name, Geom &&geometry,
                                ModelTrait &&model_trait) {

    return AddModelTrait(
        name,
        std::static_pointer_cast<IGeometrySet>(
            std::make_shared<Geom>(std::forward<Geom>(geometry))),
        std::static_pointer_cast<IModelTrait>(std::make_shared<ModelTrait>(
            std::forward<ModelTrait>(model_trait))));
  }

  BCSetT &GetModelTraitNodes() { return model_trait_nodes_; }
  const BCSetT &GetModelTraitNodes() const { return model_trait_nodes_; }
  CategorySetT &GetCategories() { return categories_; }
  const CategorySetT &GetCategories() const { return categories_; }
  const std::string &GetName() const noexcept { return name_; }

  friend fmt::formatter<CategoryNode>;

protected:
  // currently both categories, and model_traits are
  // stored with the vector backend. This is a reasonable assumption
  // with small numbers of nodes
  CategorySetT categories_;
  BCSetT model_trait_nodes_;
  std::string name_;
};
} // namespace mt
/*
template <> struct fmt::formatter<mt::CategoryNode> {
  int level = 0;
  int spaces = 2;
  constexpr auto parse(format_parse_context &ctx) {
    return parse_mt_nodes(ctx, level, spaces);
  };
  template <typename FormatContext>
  auto format(const mt::CategoryNode &cn, FormatContext &ctx) {
    auto out = format_to(ctx.out(), "{:>{}}{}", "-", level * spaces, cn.name_);
    for (auto &nd : cn.nodes_) {
      auto CN_ptr = dynamic_cast<mt::CategoryNode *>(nd.get());
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
