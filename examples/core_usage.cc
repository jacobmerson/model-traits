#include "fmt/format.h"
#include "model_traits/AssociatedModelTraits.h"
#include "model_traits/CategoryNode.h"
#include "model_traits/GeometrySet.h"
#include "model_traits/ModelTrait.h"
#include "model_traits/ModelTraits.h"
#include <cassert>

struct FunctorPlus {
  double operator()(double a, double b) const noexcept { return a + b; }
  friend std::string to_string(const FunctorPlus &f) { return f.name; }
  std::string name = "functor plus";
};
template <typename T> struct MTCast2 {
  static const T *cast(const mt::IModelTrait *mt) {
    return dynamic_cast<const T *>(mt);
  }
};
template <typename T> struct MTCast2<mt::GenericMT<T>> {
  static const mt::GenericMT<const T> *cast(const mt::IModelTrait *mt) {
    return dynamic_cast<const mt::GenericMT<const T> *>(mt);
  }
};

int main(int, char **) {
  // Create the base model traits data structure
  mt::ModelTraits model_traits{"Generic Model"};
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
  loads->AddModelTrait("load 1", mt::IdGeometrySet{1, 2, 3},
                       mt::ScalarMT{10.0});
  // add another scalar load from a geometry set initialized from a vector
  // note that we need to be careful that we don't a model trait (boundary
  // condition) with the same name to the same geometry. This will cause an
  // error when we associate the geometry
  std::vector<int> geometry_vector = {88, 99, 199};
  loads->AddModelTrait(
      "load 1",
      mt::IdGeometrySet{geometry_vector.begin(), geometry_vector.end()},
      mt::ScalarMT{15.0});

  // we can also add a model trait with geometry that stores both the dimension
  // and the id
  loads->AddModelTrait("load 2", mt::DimGeometrySet{{0, 1}, {2, 2}, {2, 3}},
                       mt::ScalarMT{7.0});

  // now lets add a model trait that uses a function. The Exprtk function class
  // to add a string function evaluation. Here, we will show adding a lambda.
  // The same method can be used for a functor, or function pointer. For
  // serialization the function is required to have a name associated with it.
  // This is accomplished by using the "NamedFunction" type.

  auto lambda_plus = mt::NamedFunction<double(double, double)>{
      "lambda plus", [](double a, double b) { return a + b; }};
  // a functor can also be used. If the functor has a to_string function, it can
  // be created without supplying a name on construction
  auto functor_plus = mt::NamedFunction<double(double, double)>{FunctorPlus{}};
  // but we don't need to explicitly construct values, we can use the typedefs
  // in ModelTrait.h to add the functions directly

  loads->AddModelTrait("function load", mt::DimGeometrySet{{0, 2}},
                       mt::ScalarFunctionMT<2>{FunctorPlus{}});
  loads->AddModelTrait(
      "function load", mt::DimGeometrySet{{0, 3}},
      mt::ScalarFunctionMT<1>{{"single input", [](double a) { return a; }}});

  // lets make sure we don't write outputs with a bool trait
  auto *output_defn = case1->FindCategoryNode("output");
  output_defn->AddModelTrait("write to file", mt::IdGeometrySet{0},
                             mt::BoolMT{false});
  // and let's set the direction of some output with a vector
  output_defn->AddModelTrait("my vector output", mt::IdGeometrySet{8},
                             mt::VectorMT{{1.0, 2.0, 3.0}});

  // lets associate the geometry so that we can search by the geometry.
  // Note that when we associate the model, we choose which geometry type to
  // include

  // first let's associate the IdGeometry
  mt::AssociatedModelTraits<int> id_associated_traits{case1};
  // now let's associate the dimensional geometry but only on the problem
  // definition
  mt::AssociatedModelTraits<mt::DimGeometry> dim_associated_traits{
      problem_definition};

  // lets see if we need to write to the file in the output
  auto *p = id_associated_traits.Find({0});
  // we put the geometry in, so we better find it
  assert(p != nullptr);
  auto *write_to_file_trait =
      p->FindCategory("output")->FindModelTrait("write to file");
  assert(write_to_file_trait != nullptr);
  // now we need to cast the trait into the appropriate type and we can call it
  auto *write_to_file_trait_casted =
      dynamic_cast<const mt::BoolMT *>(write_to_file_trait);
  assert(write_to_file_trait_casted != nullptr);
  // now lets get the value using the () operator of the model trait
  [[maybe_unused]] bool write_to_file = (*write_to_file_trait_casted)();
  assert(write_to_file == false);
  // this procedure can be made slightly more convenient by using the MTCast
  // function
  [[maybe_unused]] bool write_to_file2 =
      (*mt::MTCast<mt::BoolMT>(write_to_file_trait))();
  assert(write_to_file2 == false);

  // now lets get the function load boundary condition and evaluate it
  auto *geom_0_2 = dim_associated_traits.Find({0, 2});
  assert(geom_0_2 != nullptr);
  auto *associated_load = geom_0_2->FindCategory("loads");
  assert(associated_load != nullptr);
  // get the function and cast it to the correct type  of model trait
  // auto* function =
  // dynamic_cast<mt::ScalarFunctionMT<2>>(associated_load->FindModelTrait("function
  // load"));
  const auto *function = mt::MTCast<mt::ScalarFunctionMT<2>>(
      associated_load->FindModelTrait("function load"));
  assert(function != nullptr);
  // now lets evaluate it by getting the function as the data in the model trait
  // and calling the functions () operator
  double value = (*function)()(1.0, 2.0);
  assert(fabs(value - 3.0) < 1E-15);

  return 0;
}
