#include "bcCategoryNode.h"
#include "bcModelTraits.h"
#include "fmt/format.h"

struct Tensor {};
int main(int argc, char **argv) {
  bc::ModelTraits model_traits{"Generic Model"};
  // auto case1 = static_cast<bc::CategoryNode*>(model_traits.AddCase("case1"));
  auto case1 = model_traits.AddCase("case1");
  auto ss = case1->AddCategory("solution strategy");
  auto pd = case1->AddCategory("problem definition");
  auto o = case1->AddCategory("output");
  auto loads = ss->AddCategory("loads");
  auto bc1 = loads->AddBoundaryCondition<int, Tensor>(
      "load BC", std::initializer_list<int>{1, 3, 4, 5}, Tensor{});
  fmt::print("{}\n", model_traits);
  return 0;
}
