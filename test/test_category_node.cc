#include "bcBC.h"
#include "bcBCNode.h"
#include "bcCategoryNode.h"
#include "bcTypedefs.h"
#include "catch2/catch_test_macros.hpp"
// boundary condition struct
// using BC = mt::BoolMT;
//
using mt::BoolMT;
using mt::DimGeometrySet;
using mt::GeometrySet;

TEST_CASE("add to category node", "[node]") {
  mt::CategoryNode cn{"solution strategy"};
  REQUIRE(cn.GetName() == "solution strategy");
  mt::INode *node1 = nullptr;
  mt::INode *node2 = nullptr;
  SECTION("Category First") {
    REQUIRE_NOTHROW(node1 = cn.AddCategory("category 2"));
    REQUIRE_NOTHROW(node2 = cn.AddCategory("category 3"));
    REQUIRE(node1 != node2);
    // trying to add two category nodes with the same name returns a pointer
    // to the same node as the original
    REQUIRE_NOTHROW(node2 = cn.AddCategory("category 2"));
    REQUIRE(node2 == node1);
    // cannot add a mt with the same name as the category
    REQUIRE_NOTHROW(
        node1 = cn.AddBoundaryCondition(
            "category 2", DimGeometrySet<>(mt::DimGeometry(1, 1)), BoolMT{}));
    REQUIRE(node1 != node2);

    REQUIRE_NOTHROW(
        node1 = cn.AddBoundaryCondition("mt 1", GeometrySet<>(1), BoolMT{}));
    REQUIRE_NOTHROW(
        node2 = cn.AddBoundaryCondition("mt 2", GeometrySet<>(1), BoolMT{}));
    REQUIRE(node1 != node2);
    /*
    REQUIRE_THROWS(
        node2 = cn.AddBoundaryCondition("mt 1", GeometrySet<>(1), BoolMT{}));
    */
  }
  SECTION("mt first") {
    mt::BCNode *bc1 = nullptr;
    mt::BCNode *bc2 = nullptr;
    REQUIRE_NOTHROW(
        bc1 = cn.AddBoundaryCondition("mt 1", GeometrySet<>(1), BoolMT{}));
    REQUIRE_NOTHROW(
        bc2 = cn.AddBoundaryCondition("mt 2", GeometrySet<>(1), BoolMT{}));
    REQUIRE(bc1 != bc2);
    REQUIRE_NOTHROW(
        bc1 = cn.AddBoundaryCondition("mt 2", GeometrySet<>(1), BoolMT{}));
    REQUIRE(bc1 == bc2);
    REQUIRE_NOTHROW(
        cn.AddBoundaryCondition("category 2", GeometrySet<>(1), BoolMT{}));

    REQUIRE_NOTHROW(cn.AddCategory("category 2"));
    REQUIRE_NOTHROW(cn.AddCategory("category 3"));
  }
}
