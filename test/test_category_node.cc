#include "catch2/catch_test_macros.hpp"
#include "model_traits/CategoryNode.h"
#include "model_traits/ModelTrait.h"
#include "model_traits/ModelTraitNode.h"
#include "model_traits/Typedefs.h"
using mt::BoolMT;
using mt::CategoryNode;
using mt::DimGeometrySet;
using mt::GeometrySet;
using mt::IntMT;
using mt::ModelTraitNode;

TEST_CASE("add to category node", "[node]") {
  CategoryNode cn{"solution strategy"};
  REQUIRE(cn.GetName() == "solution strategy");
  CategoryNode *node1 = nullptr;
  CategoryNode *node2 = nullptr;
  SECTION("Category First") {
    REQUIRE_NOTHROW(node1 = cn.AddCategory("category 2"));
    // trying to add two category nodes with the same name returns a pointer
    // to the same node as the original
    REQUIRE_NOTHROW(node2 = cn.AddCategory("category 2"));
    REQUIRE(node2 == node1);
    REQUIRE(cn.FindCategoryNode("category 2") == node1);

    REQUIRE_NOTHROW(node2 = cn.AddCategory("category 3"));
    REQUIRE(node1 != node2);
    REQUIRE_NOTHROW(node2 = cn.AddCategory("category 2"));
    ModelTraitNode *mt_node1 = nullptr;
    ModelTraitNode *mt_node2 = nullptr;
    REQUIRE_NOTHROW(
        mt_node1 = cn.AddModelTrait(
            "category 2", DimGeometrySet<>(mt::DimGeometry(1, 1)), BoolMT{}));

    REQUIRE_NOTHROW(mt_node1 =
                        cn.AddModelTrait("mt 1", GeometrySet<>(1), BoolMT{}));
    REQUIRE_NOTHROW(mt_node2 =
                        cn.AddModelTrait("mt 2", GeometrySet<>(1), BoolMT{}));
    REQUIRE(mt_node1 != mt_node2);
    /*
    REQUIRE_THROWS(
        node2 = cn.AddModelTrait("mt 1", GeometrySet<>(1), BoolMT{}));
    */
  }
  SECTION("mt first") {
    mt::ModelTraitNode *bc1 = nullptr;
    mt::ModelTraitNode *bc2 = nullptr;
    REQUIRE_NOTHROW(bc1 = cn.AddModelTrait("mt 1", GeometrySet<>(1), BoolMT{}));
    REQUIRE_NOTHROW(bc2 = cn.AddModelTrait("mt 2", GeometrySet<>(1), BoolMT{}));
    REQUIRE(bc1 != bc2);
    REQUIRE_NOTHROW(bc1 = cn.AddModelTrait("mt 2", GeometrySet<>(1), BoolMT{}));
    REQUIRE(bc1 == bc2);
    REQUIRE_NOTHROW(cn.AddModelTrait("category 2", GeometrySet<>(1), BoolMT{}));

    REQUIRE_NOTHROW(cn.AddCategory("category 2"));
    REQUIRE_NOTHROW(cn.AddCategory("category 3"));
  }
  // adding multiple model traits with the same name should
  // append the geometry and model trait to the model trait node
  SECTION("multiple model traits with same name") {
    cn.AddModelTrait("model_trait", GeometrySet<>(1), BoolMT{});
    cn.AddModelTrait("model_trait", GeometrySet<>(2), BoolMT{});
    cn.AddModelTrait("model_trait", GeometrySet<>(3), IntMT{});
    ModelTraitNode *nd = cn.FindModelTraitNode("model_trait");
    REQUIRE(nd->GetModelTraits().size() == 3);
    REQUIRE(nd->GetName() == "model_trait");
    int count = 0;
    for (auto &trait_pair : nd->GetModelTraits()) {
      auto *geometry = dynamic_cast<GeometrySet<> *>(trait_pair.first.get());
      REQUIRE(geometry != nullptr);
      REQUIRE(*(geometry->begin()) == ++count);
    }
  }
}
