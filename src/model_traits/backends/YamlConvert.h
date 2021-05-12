#ifndef MODEL_TRAITS_BACKEND_YAML_CPP_CONVERT_H_
#define MODEL_TRAITS_BACKEND_YAML_CPP_CONVERT_H_
#include "model_traits/ExprtkFunction.h"
#include "model_traits/ModelTrait.h"
#include "model_traits/ModelTraitsIO.h"
#include "model_traits/NamedFunction.h"
#include "model_traits/TypeTraits.h"
#include <algorithm>
#include <exception>
#include <yaml-cpp/yaml.h>

namespace mt {
// Is there a better way to get this compile time
// type map defined for the convert function to use?
// Granted for YAML it is easy because everything goes
// to a node.
template <typename T> struct BackendTypeMap<T, YAML> {
  using type = ::YAML::Node;
};

// want to use sfinae to avoid needing to write all the function
// exporters which will largely be identical to each other, so
// we explicitly specialize the convert struct for YAML
template <> struct convert<YAML> {
  // template specialization in cpp file
  // encoding needs to be a template so we can distinguish between return types
  template <typename R, std::enable_if_t<!IsFunctionMT<R>::value, bool> = false>
  static R encode(const ::YAML::Node &, YAML *);
  // overloads for scalar, vector and tensor functions
  // SFINAE is used since the method to encode the functions of each type is
  // identical
  template <
      typename R,
      std::enable_if_t<IsFunctionMT<R>::value && (R::dim == 0), bool> = false>
  static R encode(const ::YAML::Node & /*nd*/, YAML * /*unused*/);
  template <
      typename R,
      std::enable_if_t<IsFunctionMT<R>::value && (R::dim == 1), bool> = false>
  static R encode(const ::YAML::Node & /*nd*/, YAML * /*unused*/);
  template <
      typename R,
      std::enable_if_t<IsFunctionMT<R>::value && (R::dim == 2), bool> = false>
  static R encode(const ::YAML::Node & /*nd*/, YAML * /*unused*/);

  static void decode(const BoolMT &, ::YAML::Node &, YAML *);
  static void decode(const MatrixMT &, ::YAML::Node &, YAML *);
  static void decode(const ScalarMT &, ::YAML::Node &, YAML *);
  static void decode(const IntMT &, ::YAML::Node &, YAML *);
  static void decode(const StringMT &, ::YAML::Node &, YAML *);
  static void decode(const VectorMT &, ::YAML::Node &, YAML *);
  static void decode(const IdGeometrySet &, ::YAML::Node &, YAML *);
  static void decode(const DimIdGeometrySet &, ::YAML::Node &, YAML *);
  static void decode(const ModelTraitNode &bcn, ::YAML::Node &, YAML *);
  static void decode(const CategoryNode &, ::YAML::Node &, YAML *);
  static void decode(const ModelTraits &, ::YAML::Node &, YAML *);
  static void decode(const VoidMT &, ::YAML::Node &, YAML *);

  template <typename T,
            std::enable_if_t<IsNamedFunction<T>::value, bool> = false>
  static void decode(const GenericMT<T, 0> & /*mt*/, ::YAML::Node & /*nd*/,
                     YAML * /*unused*/);

  template <typename T,
            std::enable_if_t<IsNamedFunction<T>::value, bool> = false>
  static void decode(const GenericMT<T, 1> & /*mt*/, ::YAML::Node & /*nd*/,
                     YAML * /*unused*/);

  template <typename T,
            std::enable_if_t<IsNamedFunction<T>::value, bool> = false>
  static void decode(const GenericMT<T, 2> & /*mt*/, ::YAML::Node & /*nd*/,
                     YAML * /*unused*/);
};

template <typename T, std::enable_if_t<IsNamedFunction<T>::value, bool>>
void convert<YAML>::decode(const GenericMT<T, 0> &bc, ::YAML::Node &nd,
                           YAML * /*unused*/) {
  auto eqn_name = to_string(bc);
  auto count = std::count(begin(eqn_name), end(eqn_name), '$');
  // if there are any variables in the name denoted by "$" it's an expression
  // otherwise it is a function type, like a lambda of some sort.
  nd["type"] = count > 0 ? "expression" : "function";
  nd["value"] = std::move(eqn_name);
  nd["num variables"] = T::ArgsCount();
}
template <typename T, std::enable_if_t<IsNamedFunction<T>::value, bool>>
void convert<YAML>::decode(const GenericMT<T, 1> &bc, ::YAML::Node &nd,
                           YAML * /*unused*/) {
  auto count = 0;
  auto val = nd["value"];
  val.SetStyle(::YAML::EmitterStyle::Flow);
  for (size_t i = 0; i < bc.size(); ++i) {
    auto eqn_name = to_string(bc(i));
    count += std::count(begin(eqn_name), end(eqn_name), '$');
    val.push_back(std::move(eqn_name));
  }
  nd["type"] = count > 1 ? "vector expression" : "vector function";
  nd["num variables"] = T::ArgsCount();
}
template <typename T, std::enable_if_t<IsNamedFunction<T>::value, bool>>
void convert<YAML>::decode(const GenericMT<T, 2> &bc, ::YAML::Node &nd,
                           YAML * /*unused*/) {
  auto count = 0;
  auto val = nd["value"];
  val.SetStyle(::YAML::EmitterStyle::Flow);
  for (int i = 0; i < bc.nrows(); ++i) {
    std::vector<std::string> row;
    row.reserve(bc.ncols());
    for (int j = 0; j < bc.ncols(); ++j) {
      auto eqn_name = to_string(bc(i, j));
      count += std::count(begin(eqn_name), end(eqn_name), '$');
      row.push_back(std::move(eqn_name));
    }
    val.push_back(row);
  }
  nd["type"] = count > 1 ? "matrix expression" : "matrix function";
  nd["num variables"] = T::ArgsCount();
}

template <typename R,
          std::enable_if_t<IsFunctionMT<R>::value && (R::dim == 0), bool>>
R convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  return R{ExprtkFunction<R::type::ArgsCount()>{nd.as<std::string>()}};
}
template <typename R,
          std::enable_if_t<IsFunctionMT<R>::value && (R::dim == 1), bool>>
R convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  std::vector<typename R::type> functions;
  for (const auto &v : nd) {
    functions.emplace_back(
        ExprtkFunction<R::type::ArgsCount()>{v.as<std::string>()});
  }
  return R{std::move(functions)};
}
template <typename R,
          std::enable_if_t<IsFunctionMT<R>::value && (R::dim == 2), bool>>
R convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  std::vector<std::vector<typename R::type>> functions;
  for (const auto &row : nd) {
    functions.emplace_back();
    for (const auto &val : row) {
      functions.back().emplace_back(
          ExprtkFunction<R::type::ArgsCount()>{val.as<std::string>()});
    }
  }
  return R{std::move(functions)};
}

} // namespace mt

#endif
