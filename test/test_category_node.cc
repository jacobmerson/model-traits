#include "catch2/catch_test_macros.hpp"
#include "model_traits/CategoryNode.h"
#include "model_traits/ModelTrait.h"
#include "model_traits/ModelTraitNode.h"
#include "model_traits/Typedefs.h"
using mt::BoolMT;
using mt::CategoryNode;
using mt::DimIdGeometrySet;
using mt::IdGeometrySet;
using mt::IntMT;
using mt::ModelTraitNode;

TEST_CASE("add to category node", "[node]") {
  CategoryNode cn{"solution strategy", ""};
  REQUIRE(cn.GetType() == "solution strategy");
  CategoryNode *node1 = nullptr;
  CategoryNode *node2 = nullptr;
  SECTION("Named Categories") {
    REQUIRE_NOTHROW(node2 = cn.AddCategory("category 2", "flamingo"));
    REQUIRE(node2 != nullptr);
    REQUIRE_NOTHROW(node1 = cn.FindCategoryNodeByType("category 2"));
    REQUIRE(node1 != nullptr);
    REQUIRE(node1 == node2);
    REQUIRE(node2->GetType() == "category 2");
    REQUIRE(node2->GetName() == "flamingo");

    REQUIRE_NOTHROW(node1 = cn.AddCategory("category 2"));
    REQUIRE(cn.GetCategoryNodes().size() == 2);
    REQUIRE(node1 != nullptr);
    REQUIRE(cn.FindCategoryNode("category 2", "") == node1);
    REQUIRE(cn.FindCategoryNode("category 2", "") !=
            cn.FindCategoryNode("category 2", "flamingo"));

    REQUIRE(node1->GetType() == "category 2");
    REQUIRE(node1->GetName().empty());

    REQUIRE_NOTHROW(node1 = cn.FindCategoryNodeByType("category 2"));
    REQUIRE(node1 != nullptr);
    REQUIRE((node1 == cn.FindCategoryNode("category 2", "") ||
             node1 == cn.FindCategoryNode("category 2", "flamingo")));

    auto nodes = cn.FindCategoryNodesByType("category 2");
    REQUIRE(nodes.size() == 2);
  }
  SECTION("Category First") {
    REQUIRE_NOTHROW(node1 = cn.AddCategory("category 2"));
    // trying to add two category nodes with the same name returns a pointer
    // to the same node as the original
    REQUIRE_NOTHROW(node2 = cn.AddCategory("category 2"));
    REQUIRE(node2 == node1);
    REQUIRE(cn.FindCategoryNodeByType("category 2") == node1);

    REQUIRE_NOTHROW(node2 = cn.AddCategory("category 3"));
    REQUIRE(node1 != node2);
    REQUIRE_NOTHROW(node2 = cn.AddCategory("category 2"));
    ModelTraitNode *mt_node1 = nullptr;
    ModelTraitNode *mt_node2 = nullptr;
    REQUIRE_NOTHROW(
        mt_node1 = cn.AddModelTrait(
            "category 2", DimIdGeometrySet(mt::DimIdGeometry(1, 1)), BoolMT{}));

    REQUIRE_NOTHROW(mt_node1 =
                        cn.AddModelTrait("mt 1", IdGeometrySet(1), BoolMT{}));
    REQUIRE_NOTHROW(mt_node2 =
                        cn.AddModelTrait("mt 2", IdGeometrySet(1), BoolMT{}));
    REQUIRE(mt_node1 != mt_node2);

    auto nodes = GetCategoriesByType(&cn, "category 2");
    REQUIRE(nodes.size() == 1);
    REQUIRE(nodes[0] == node2);
  }
  SECTION("mt first") {
    mt::ModelTraitNode *bc1 = nullptr;
    mt::ModelTraitNode *bc2 = nullptr;
    REQUIRE_NOTHROW(bc1 = cn.AddModelTrait("mt 1", IdGeometrySet(1), BoolMT{}));
    REQUIRE_NOTHROW(bc2 = cn.AddModelTrait("mt 2", IdGeometrySet(1), BoolMT{}));
    REQUIRE(bc1 != bc2);
    REQUIRE_NOTHROW(bc1 = cn.AddModelTrait("mt 2", IdGeometrySet(1), BoolMT{}));
    REQUIRE(bc1 == bc2);
    REQUIRE_NOTHROW(cn.AddModelTrait("category 2", IdGeometrySet(1), BoolMT{}));

    REQUIRE_NOTHROW(cn.AddCategory("category 2"));
    REQUIRE_NOTHROW(cn.AddCategory("category 3"));
  }
  // adding multiple model traits with the same name should
  // append the geometry and model trait to the model trait node
  SECTION("multiple model traits with same name") {
    cn.AddModelTrait("model_trait", IdGeometrySet(1), BoolMT{});
    cn.AddModelTrait("model_trait", IdGeometrySet(2), BoolMT{});
    cn.AddModelTrait("model_trait", IdGeometrySet(3), IntMT{});
    ModelTraitNode *nd = cn.FindModelTraitNode("model_trait");
    REQUIRE(nd->GetModelTraits().size() == 3);
    REQUIRE(nd->GetName() == "model_trait");
    int count = 0;
    for (auto &trait_pair : nd->GetModelTraits()) {
      auto *geometry = dynamic_cast<IdGeometrySet *>(trait_pair.first.get());
      REQUIRE(geometry != nullptr);
      REQUIRE(*(geometry->begin()) == ++count);
    }
  }
}
