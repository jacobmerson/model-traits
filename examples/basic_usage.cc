#include "model_traits/AssociatedModelTraits.h"
#include "model_traits/ExprtkFunction.h"
#include "model_traits/GeometrySet.h"
#include "model_traits/ModelTrait.h"
#include "model_traits/ModelTraits.h"
#include <fmt/format.h>

class FunctorPlus {
public:
  double operator()(double a) const noexcept { return 4 * a; }
  friend std::string to_string(const FunctorPlus &f) { return f.name; }

private:
  std::string name = "functor multiply by 4";
};

using mt::BoolMT;
using mt::DimIdGeometry;
using mt::DimIdGeometrySet;
using mt::ExprtkFunction;
using mt::IdGeometrySet;
using mt::IntFunctionMT;
using mt::IntMT;
using mt::ModelTraits;
using mt::MTCast;
using mt::ScalarFunctionMT;
using mt::ScalarMT;
using mt::VectorMT;

int main(int, char **) {
  // Step 1: load in simulation data
  ModelTraits model_traits{"Generic Model"};
  auto *case1 = model_traits.AddCase("case1");
  case1->AddCategory("solution strategy");
  case1->AddCategory("problem definition");
  case1->AddCategory("output");

  auto *problem_definition = case1->FindCategoryNode("problem definition");
  auto *loads = problem_definition->AddCategory("loads");
  std::vector<DimIdGeometry> v = {{0, 1}, {2, 2}, {2, 3}};
  loads->AddModelTrait("load 2", v, ScalarMT{7.0});
  loads->AddModelTrait("load 3", v, VectorMT{{1, 2, 3}});

  // functor function
  loads->AddModelTrait("functor function", DimIdGeometrySet{{0, 2}},
                       ScalarFunctionMT<1>{FunctorPlus{}});

  // lambda function
  loads->AddModelTrait("lambda function", DimIdGeometrySet{{0, 2}},
                       ScalarFunctionMT<1>{{"lambda multiply by 4",
                                            [](double a) { return 4 * a; }}});

  // string function
  loads->AddModelTrait("exprtk function", DimIdGeometrySet{{0, 2}},
                       ScalarFunctionMT<1>{ExprtkFunction<1>("4*$x")});

  // Step 2: After the model traits are fully loaded, the model is associated,
  // so that it can be quickly queried by geometric entity

  auto dim_associated_traits = AssociateModel(problem_definition);
  // now lets get the function load boundary condition and evaluate it
  const auto *geom_0_2 = dim_associated_traits->Find({0, 2});
  const auto *associated_load = geom_0_2->FindCategory("loads");

  const auto *functor_function = MTCast<ScalarFunctionMT<1>>(
      associated_load->FindModelTrait("functor function"));
  const auto *lambda_function = MTCast<ScalarFunctionMT<1>>(
      associated_load->FindModelTrait("lambda function"));
  const auto *exprtk_function = MTCast<ScalarFunctionMT<1>>(
      associated_load->FindModelTrait("exprtk function"));
  fmt::print("{}:{}, {}:{}, {}:{}\n", to_string(*functor_function),
             (*functor_function)(1.0), to_string(*lambda_function),
             (*lambda_function)(1.0), to_string(*exprtk_function),
             (*exprtk_function)(1.0));

  // let's get the other two load boundary conditions
  const auto *geom_2_2 = dim_associated_traits->Find({2, 2});
  associated_load = geom_2_2->FindCategory("loads");
  const auto *load2 =
      MTCast<ScalarMT>(associated_load->FindModelTrait("load 2"));
  const auto *load3 =
      MTCast<VectorMT>(associated_load->FindModelTrait("load 3"));
  fmt::print("load 2: {}.\n", (*load2)());
  fmt::print("load 3: [{},{},{}].\n", (*load3)(0), (*load3)(1), (*load3)(2));

  return 0;
}
