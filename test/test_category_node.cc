#include "catch2/catch_test_macros.hpp"
#include "model_traits/CategoryNode.h"
#include "model_traits/ModelTrait.h"
#include "model_traits/ModelTraitNode.h"
#include "model_traits/Typedefs.h"
// boundary condition struct
// using BC = mt::BoolMT;
//
using mt::BoolMT;
using mt::CategoryNode;
using mt::DimGeometrySet;
using mt::GeometrySet;
using mt::ModelTraitNode;

TEST_CASE("add to category node", "[node]") {
  CategoryNode cn{"solution strategy"};
  REQUIRE(cn.GetName() == "solution strategy");
  CategoryNode *node1 = nullptr;
  CategoryNode *node2 = nullptr;
  SECTION("Category First") {
    REQUIRE_NOTHROW(node1 = cn.AddCategory("category 2"));
    REQUIRE_NOTHROW(node2 = cn.AddCategory("category 3"));
    REQUIRE(node1 != node2);
    // trying to add two category nodes with the same name returns a pointer
    // to the same node as the original
    REQUIRE_NOTHROW(node2 = cn.AddCategory("category 2"));
    REQUIRE(node2 == node1);
    ModelTraitNode *mt_node1 = nullptr;
    ModelTraitNode *mt_node2 = nullptr;
    // cannot add a mt with the same name as the category
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
}
