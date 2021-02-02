#include "bcBCNode.h"
#include "bcCategoryNode.h"
#include "bcTypedefs.h"
#include "catch2/catch_test_macros.hpp"
// boundary condition struct
struct BC {};

TEST_CASE("add to category node", "[node]") {
  bc::CategoryNode cn{"solution strategy"};
  REQUIRE(cn.GetName() == "solution strategy");
  auto bc1 =
      std::make_unique<bc::BCNode<bc::OrdinalType, bc::OrdinalType>>("bc1");
  auto bc2 =
      std::make_unique<bc::BCNode<bc::OrdinalType, bc::OrdinalType>>("bc2");
  auto bc2a =
      std::make_unique<bc::BCNode<bc::OrdinalType, bc::OrdinalType>>("bc2");
  auto bcCat = std::make_unique<bc::BCNode<bc::OrdinalType, bc::OrdinalType>>(
      "category 2");
  // auto cn2 = std::make_unique<bc::CategoryNode>("category 2");
  // auto cn2a = std::make_unique<bc::CategoryNode>("category 2");
  // auto cn3 = std::make_unique<bc::CategoryNode>("category 3");
  bc::INode *node1 = nullptr;
  bc::INode *node2 = nullptr;
  SECTION("Category First") {
    REQUIRE_NOTHROW(node1 = cn.AddCategory("category 2"));
    REQUIRE_NOTHROW(node2 = cn.AddCategory("category 3"));
    REQUIRE(node1 != node2);
    // trying to add two category nodes with the same name throws
    REQUIRE_THROWS(node2 = cn.AddCategory("category 2"));
    // cannot add a bc with the same name as the category
    REQUIRE_THROWS(cn.AddBoundaryCondition<int, BC>("category 2", 1, BC{}));

    REQUIRE_NOTHROW(node1 = cn.AddBoundaryCondition<int, BC>("bc 1", 1, BC{}));
    REQUIRE_NOTHROW(node2 = cn.AddBoundaryCondition<int, BC>("bc 2", 1, BC{}));
    REQUIRE(node1 != node2);
    REQUIRE_THROWS(node2 = cn.AddBoundaryCondition<int, BC>("bc 1", 1, BC{}));
  }
  SECTION("bc first") {
    REQUIRE_NOTHROW(cn.AddBoundaryCondition<int, BC>("bc 1", 1, BC{}));
    REQUIRE_NOTHROW(cn.AddBoundaryCondition<int, BC>("bc 2", 1, BC{}));
    REQUIRE_THROWS(cn.AddBoundaryCondition<int, BC>("bc 2", 1, BC{}));
    REQUIRE_NOTHROW(cn.AddBoundaryCondition<int, BC>("category 2", 1, BC{}));

    REQUIRE_THROWS(cn.AddCategory("category 2"));
    REQUIRE_NOTHROW(cn.AddCategory("category 3"));
  }
}
