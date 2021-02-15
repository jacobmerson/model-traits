#include "bcBC.h"
#include "bcBCNode.h"
#include "bcCategoryNode.h"
#include "bcTypedefs.h"
#include "catch2/catch_test_macros.hpp"
// boundary condition struct
// using BC = bc::BoolBC;
//
using bc::BoolBC;
using bc::DimGeometrySet;
using bc::GeometrySet;

TEST_CASE("add to category node", "[node]") {
  bc::CategoryNode cn{"solution strategy"};
  REQUIRE(cn.GetName() == "solution strategy");
  bc::INode *node1 = nullptr;
  bc::INode *node2 = nullptr;
  SECTION("Category First") {
    REQUIRE_NOTHROW(node1 = cn.AddCategory("category 2"));
    REQUIRE_NOTHROW(node2 = cn.AddCategory("category 3"));
    REQUIRE(node1 != node2);
    // trying to add two category nodes with the same name throws
    REQUIRE_THROWS(node2 = cn.AddCategory("category 2"));
    // cannot add a bc with the same name as the category
    REQUIRE_THROWS(cn.AddBoundaryCondition(
        "category 2", DimGeometrySet<>(bc::DimGeometry(1, 1)), BoolBC{}));

    REQUIRE_NOTHROW(
        node1 = cn.AddBoundaryCondition("bc 1", GeometrySet<>(1), BoolBC{}));
    REQUIRE_NOTHROW(
        node2 = cn.AddBoundaryCondition("bc 2", GeometrySet<>(1), BoolBC{}));
    REQUIRE(node1 != node2);
    REQUIRE_THROWS(
        node2 = cn.AddBoundaryCondition("bc 1", GeometrySet<>(1), BoolBC{}));
  }
  SECTION("bc first") {
    REQUIRE_NOTHROW(
        cn.AddBoundaryCondition("bc 1", GeometrySet<>(1), BoolBC{}));
    REQUIRE_NOTHROW(
        cn.AddBoundaryCondition("bc 2", GeometrySet<>(1), BoolBC{}));
    REQUIRE_THROWS(cn.AddBoundaryCondition("bc 2", GeometrySet<>(1), BoolBC{}));
    REQUIRE_NOTHROW(
        cn.AddBoundaryCondition("category 2", GeometrySet<>(1), BoolBC{}));

    REQUIRE_THROWS(cn.AddCategory("category 2"));
    REQUIRE_NOTHROW(cn.AddCategory("category 3"));
  }
}
