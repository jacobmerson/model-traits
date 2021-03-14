#include "fmt/format.h"
#include "model_traits/bcBC.h"
#include "model_traits/bcCategoryNode.h"
#include "model_traits/bcGeometrySet.h"
#include "model_traits/bcModelTraits.h"

int main(int, char **) {
  mt::ModelTraits model_traits{"Generic Model"};
  // auto case1 = static_cast<mt::CategoryNode*>(model_traits.AddCase("case1"));
  auto case1 = model_traits.AddCase("case1");
  auto ss = case1->AddCategory("solution strategy");
  [[maybe_unused]] auto pd = case1->AddCategory("problem definition");
  [[maybe_unused]] auto o = case1->AddCategory("output");
  auto loads = ss->AddCategory("loads");

  std::vector<int> g{1, 2, 3, 4, 5, 6};
  [[maybe_unused]] auto bc1 = loads->AddModelTrait(
      "load BC", mt::GeometrySet<>{g.begin(), g.end()}, mt::ScalarMT{});
  fmt::print("{}\n", model_traits);
  return 0;
}
