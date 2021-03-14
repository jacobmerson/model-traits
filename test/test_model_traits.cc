#include "bcModelTraits.h"
#include "catch2/catch_test_macros.hpp"

class TestModelTraits : public mt::ModelTraits {
public:
  TestModelTraits(const std::string &name) : mt::ModelTraits(name){};
  const auto &GetName() const { return name_; }
};

TEST_CASE("add and remove case from ModelTraits", "[traits]") {
  TestModelTraits model_traits{"mytraits"};
  REQUIRE(model_traits.GetName() == "mytraits");

  // ensure that original size of cases is zero
  REQUIRE(model_traits.NumCases() == 0);

  auto case1 = model_traits.AddCase("case1");
  auto case2 = model_traits.AddCase("case2");
  // ensure that we are adding a new case
  // each time we add case
  REQUIRE(case1 != case2);
  REQUIRE(case1 == model_traits.GetCase("case1"));
  REQUIRE(case2 == model_traits.GetCase("case2"));
  REQUIRE(model_traits.GetCase("nonexistent case") == nullptr);
  // check that adding cases works
  REQUIRE(model_traits.NumCases() == 2);
  // adding an existing case throws an error
  REQUIRE_THROWS(model_traits.AddCase("case1"));
  // ensure that we don't contain random cases
  REQUIRE(model_traits.GetCase("case3") == nullptr);
  REQUIRE(model_traits.GetCase("case4") == nullptr);

  REQUIRE(model_traits.RemoveCase("case1") != nullptr);
  REQUIRE(model_traits.GetCase("case1") == nullptr);
  REQUIRE(model_traits.NumCases() == 1);
  REQUIRE(model_traits.RemoveCase("case1") == nullptr);
  REQUIRE(model_traits.RemoveCase("case2") != nullptr);
  REQUIRE(model_traits.RemoveCase("case4") == nullptr);
}
