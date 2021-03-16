#include "fmt/format.h"
#include "model_traits/AssociatedModelTraits.h"
#include "model_traits/GeometrySet.h"
#include "model_traits/ModelTrait.h"
#include "model_traits/ModelTraits.h"
#include <cassert>
#include <fmt/format.h>

struct FunctorPlus {
  double operator()(double a, double b) const noexcept { return a + b; }
  friend std::string to_string(const FunctorPlus &f) { return f.name; }
  std::string name = "functor plus";
};

using mt::AssociatedModelTraits;
using mt::BoolMT;
using mt::DimGeometry;
using mt::DimGeometrySet;
using mt::IdGeometrySet;
using mt::IntMT;
using mt::ModelTraits;
using mt::MTCast;
using mt::NamedFunction;
using mt::ScalarFunctionMT;
using mt::ScalarMT;
using mt::VectorMT;

int main(int, char **) {
  // Create the base model traits data structure
  ModelTraits model_traits{"Generic Model"};
  auto *case1 = model_traits.AddCase("case1");
  case1->AddCategory("solution strategy");
  case1->AddCategory("problem definition");
  case1->AddCategory("output");

  // Since adding categories to case1 will potentially invalidate the pointer,
  // we don't directly use the output of AddCategory, but instead we find the
  // category after all the other categories in case1 are added
  auto *problem_definition = case1->FindCategoryNode("problem definition");
  // add a category to the problem definition
  auto *loads = problem_definition->AddCategory("loads");
  // this time we aren't adding any more categories to the solution strategy
  // before operating on it, so we are safe to use the output of AddCategory

  // add the first load with the geometry as an id geometry set (geometry is
  // stored as only the id with no dimension)
  loads->AddModelTrait("load 1", IdGeometrySet{1, 2, 3}, ScalarMT{10.0});
  // add another scalar load from a geometry set initialized from a vector
  // note that we need to be careful that we don't a model trait (boundary
  // condition) with the same name to the same geometry. This will cause an
  // error when we associate the geometry
  std::vector<int> geometry_vector = {88, 99, 199};
  loads->AddModelTrait(
      "load 1", IdGeometrySet{geometry_vector.begin(), geometry_vector.end()},
      ScalarMT{15.0});

  // we can also add a model trait with geometry that stores both the dimension
  // and the id
  loads->AddModelTrait("load 2", DimGeometrySet{{0, 1}, {2, 2}, {2, 3}},
                       ScalarMT{7.0});

  // now lets add a model trait that uses a function. The Exprtk function class
  // to add a string function evaluation. Here, we will show adding a lambda.
  // The same method can be used for a functor, or function pointer. For
  // serialization the function is required to have a name associated with it.
  // This is accomplished by using the "NamedFunction" type.

  auto lambda_plus = NamedFunction<double(double, double)>{
      "lambda plus", [](double a, double b) { return a + b; }};
  // a functor can also be used. If the functor has a to_string function, it can
  // be created without supplying a name on construction
  auto functor_plus = NamedFunction<double(double, double)>{FunctorPlus{}};
  // but we don't need to explicitly construct values, we can use the typedefs
  // in ModelTrait.h to add the functions directly

  loads->AddModelTrait("function load", DimGeometrySet{{0, 2}},
                       ScalarFunctionMT<2>{FunctorPlus{}});
  loads->AddModelTrait(
      "function load", DimGeometrySet{{0, 3}},
      ScalarFunctionMT<1>{{"single input", [](double a) { return a; }}});

  // lets make sure we don't write outputs with a bool trait.
  // This options isn't associated with a particular geometry, so we leave the
  // geometry set empty.
  auto *output_defn = case1->FindCategoryNode("output");
  output_defn->AddModelTrait("write to file", IdGeometrySet{}, BoolMT{false});
  // and let's set the direction of some output with a vector
  output_defn->AddModelTrait("my vector output", IdGeometrySet{8},
                             VectorMT{{1.0, 2.0, 3.0}});

  // lets associate the geometry so that we can search by the geometry.
  // Note that when we associate the model, we choose which geometry type to
  // include

  // first let's associate the IdGeometry
  AssociatedModelTraits<int> id_associated_traits{case1};
  // now let's associate the dimensional geometry but only on the problem
  // definition
  AssociatedModelTraits<DimGeometry> dim_associated_traits{problem_definition};

  // lets see if we need to write to the file in the output
  // note that since this model trait has no geometry associated with it,
  // we will use a special method to get the correct list of category nodes
  auto *p = id_associated_traits.GetNullGeometry();
  // we put the geometry in, so we better find it
  auto *write_to_file_trait =
      p->FindCategory("output")->FindModelTrait("write to file");
  // now we need to cast the trait into the appropriate type and we can call it
  auto *write_to_file_trait_casted = MTCast<BoolMT>(write_to_file_trait);

  // now lets get the value using the () operator of the model trait
  bool write_to_file = (*write_to_file_trait_casted)();
  fmt::print("Writing to file?: {}\n", write_to_file);

  // now lets get the function load boundary condition and evaluate it
  auto *geom_0_2 = dim_associated_traits.Find({0, 2});
  auto *associated_load = geom_0_2->FindCategory("loads");
  // cast the function to the appropriate model trait type
  const auto *function = MTCast<ScalarFunctionMT<2>>(
      associated_load->FindModelTrait("function load"));
  // now lets evaluate it by getting the function as the data in the model trait
  // and calling the functions () operator. Note that functions have a bit of an
  // awkward syntax where the first () gets the function as the data stored in
  // boundary condition, and second calls the underlying function. The first set
  // of parens can be removed, but this offers consistency with vectors and
  // matrices
  fmt::print("Evaluating function load f(1,2)={}.\n", (*function)()(1, 2));

  // let's see what is stored in our vector output
  assert(id_associated_traits.Find({8})->FindCategory("output")->FindModelTrait(
             "my vector output") != nullptr);
  const VectorMT &my_vec = (*MTCast<VectorMT>(
      id_associated_traits.Find({8})->FindCategory("output")->FindModelTrait(
          "my vector output")));
  fmt::print("Vector: [{}, {}, {}].\n", my_vec(0), my_vec(1), my_vec(2));

  return 0;
}
