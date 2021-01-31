#include "bcModelTraits.h"
#include "catch2/catch_test_macros.hpp"

class TestModelTraits : public bc::ModelTraits {
public:
  TestModelTraits(const std::string &name) : bc::ModelTraits(name){};
  const auto &GetCases() const { return cases_; }
  const auto &GetName() const { return name_; }
};

TEST_CASE("add and remove case from ModelTraits", "[traits]") {
  TestModelTraits model_traits{"mytraits"};
  REQUIRE(model_traits.GetName() == "mytraits");

  // ensure that original size of cases is zero
  REQUIRE(model_traits.GetCases().size() == 0);

  auto case1 = model_traits.AddCase("case1");
  auto case2 = model_traits.AddCase("case2");
  // enusure that we are adding a new case
  // each time we add case
  REQUIRE(case1 != case2);
  REQUIRE(case1->IsBoundaryCondition() == false);
  REQUIRE(case2->IsBoundaryCondition() == false);
  REQUIRE(case1 == model_traits.GetCase("case1"));
  REQUIRE(case2 == model_traits.GetCase("case2"));
  REQUIRE(model_traits.GetCase("nonexistent case") == nullptr);
  // check that adding cases works
  REQUIRE(model_traits.GetCases().size() == 2);
  // adding an existing case returns the correct INode
  REQUIRE(model_traits.AddCase("case1") == case1);
  // ensure that we correctly have the cases we added
  REQUIRE(model_traits.GetCases().count("case1") == 1);
  REQUIRE(model_traits.GetCases().count("case2") == 1);
  // ensure that we don't contain random cases
  REQUIRE(model_traits.GetCases().count("case3") == 0);
  REQUIRE(model_traits.GetCases().count("case4") == 0);

  REQUIRE(model_traits.RemoveCase("case1") == true);
  REQUIRE(model_traits.GetCases().count("case1") == 0);
  REQUIRE(model_traits.GetCases().size() == 1);
  REQUIRE(model_traits.RemoveCase("case1") == false);
  REQUIRE(model_traits.RemoveCase("case2") == true);
  REQUIRE(model_traits.RemoveCase("case4") == false);
}
