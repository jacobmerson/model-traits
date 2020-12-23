#include "bcBCNode.h"
#include "bcCategoryNode.h"
#include "bcTypedefs.h"
#include "catch2/catch_test_macros.hpp"

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
  auto cn2 = std::make_unique<bc::CategoryNode>("category 2");
  auto cn2a = std::make_unique<bc::CategoryNode>("category 2");
  auto cn3 = std::make_unique<bc::CategoryNode>("category 3");
  SECTION("Category First") {
    REQUIRE_NOTHROW(cn.AddNode(std::move(cn2)));
    REQUIRE_NOTHROW(cn.AddNode(std::move(cn3)));
    // trying to add two category nodes with the same name trhows
    REQUIRE_THROWS(cn.AddNode(std::move(cn2a)));
    REQUIRE_THROWS(cn.AddNode(std::move(bcCat)));

    REQUIRE_NOTHROW(cn.AddNode(std::move(bc1)));
    REQUIRE_NOTHROW(cn.AddNode(std::move(bc2)));
    REQUIRE_NOTHROW(cn.AddNode(std::move(bc2a)));
  }
  SECTION("bc first") {
    REQUIRE_NOTHROW(cn.AddNode(std::move(bc1)));
    REQUIRE_NOTHROW(cn.AddNode(std::move(bc2)));
    REQUIRE_NOTHROW(cn.AddNode(std::move(bc2a)));
    REQUIRE_NOTHROW(cn.AddNode(std::move(bcCat)));

    REQUIRE_THROWS(cn.AddNode(std::move(cn2)));
    REQUIRE_NOTHROW(cn.AddNode(std::move(cn3)));
    // trying to add two category nodes with the same name trhows
    REQUIRE_THROWS(cn.AddNode(std::move(cn2a)));
  }
}
