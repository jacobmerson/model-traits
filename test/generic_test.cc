#include "bcBC.h"
#include "bcCategoryNode.h"
#include "bcGeometrySet.h"
#include "bcModelTraits.h"
#include "fmt/format.h"

int main(int, char **) {
  bc::ModelTraits model_traits{"Generic Model"};
  // auto case1 = static_cast<bc::CategoryNode*>(model_traits.AddCase("case1"));
  auto case1 = model_traits.AddCase("case1");
  auto ss = case1->AddCategory("solution strategy");
  [[maybe_unused]] auto pd = case1->AddCategory("problem definition");
  [[maybe_unused]] auto o = case1->AddCategory("output");
  auto loads = ss->AddCategory("loads");

  std::vector<int> g{1, 2, 3, 4, 5, 6};
  [[maybe_unused]] auto bc1 = loads->AddBoundaryCondition(
      "load BC", bc::GeometrySet<>{g.begin(), g.end()}, bc::ScalarBC{});
  fmt::print("{}\n", model_traits);
  return 0;
}
