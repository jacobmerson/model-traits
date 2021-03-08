#ifndef BC_BACKEND_YAML_CPP_CONVERT_H__
#define BC_BACKEND_YAML_CPP_CONVERT_H__
#include "bcBackendYamlCpp.h"
#include "bcTypeTraits.h"
#include <yaml-cpp/yaml.h>
//#include "bcConvert.h"
#include "bcBC.h"
#include "bcExprtkFunction.h"
#include "bcNamedFunction.h"
#include <exception>

namespace bc {
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
  template <typename R, std::enable_if_t<!IsFunctionBC<R>::value, bool> = false>
  static R encode(const ::YAML::Node &, YAML *);
  // overloads for scalar, vector and tensor functions
  // SFINAE is used since the method to encode the functions of each type is
  // identical
  template <
      typename R,
      std::enable_if_t<IsFunctionBC<R>::value && (R::dim == 0), bool> = false>
  static R encode(const ::YAML::Node & /*nd*/, YAML * /*unused*/);
  template <
      typename R,
      std::enable_if_t<IsFunctionBC<R>::value && (R::dim == 1), bool> = false>
  static R encode(const ::YAML::Node & /*nd*/, YAML * /*unused*/);
  template <
      typename R,
      std::enable_if_t<IsFunctionBC<R>::value && (R::dim == 2), bool> = false>
  static R encode(const ::YAML::Node & /*nd*/, YAML * /*unused*/);

  using DimSetT = GeometrySet<DimGeometry, BC_VEC_WORKAROUND>;
  using SetT = GeometrySet<OrdinalType, BC_VEC_WORKAROUND>;

  static void decode(const BoolBC &, ::YAML::Node &, YAML *);
  static void decode(const MatrixBC &, ::YAML::Node &, YAML *);
  static void decode(const ScalarBC &, ::YAML::Node &, YAML *);
  static void decode(const IntBC &, ::YAML::Node &, YAML *);
  static void decode(const StringBC &, ::YAML::Node &, YAML *);
  static void decode(const VectorBC &, ::YAML::Node &, YAML *);
  static void decode(const SetT &, ::YAML::Node &, YAML *);
  static void decode(const DimSetT &, ::YAML::Node &, YAML *);
  static void decode(const BCNode &bcn, ::YAML::Node &, YAML *);
  static void decode(const CategoryNode &, ::YAML::Node &, YAML *);
  static void decode(const ModelTraits &, ::YAML::Node &, YAML *);

  template <typename T,
            std::enable_if_t<IsNamedFunction<T>::value, bool> = false>
  static void decode(const GenericBC<T, 0> & /*bc*/, ::YAML::Node & /*nd*/,
                     YAML * /*unused*/);

  template <typename T,
            std::enable_if_t<IsNamedFunction<T>::value, bool> = false>
  static void decode(const GenericBC<T, 1> & /*bc*/, ::YAML::Node & /*nd*/,
                     YAML * /*unused*/);

  template <typename T,
            std::enable_if_t<IsNamedFunction<T>::value, bool> = false>
  static void decode(const GenericBC<T, 2> & /*bc*/, ::YAML::Node & /*nd*/,
                     YAML * /*unused*/);
};

template <typename T, std::enable_if_t<IsNamedFunction<T>::value, bool>>
void convert<YAML>::decode(const GenericBC<T, 0> &bc, ::YAML::Node &nd,
                           YAML * /*unused*/) {
  auto eqn_name = to_string(bc);
  auto count = std::count(begin(eqn_name), end(eqn_name), '$');
  // if there are any variables in the name denoted by "$" it's an expression
  // otherwise it is a function type, like a lambda of some sort.
  nd["type"] = count > 1 ? "expression" : "function";
  nd["value"] = std::move(eqn_name);
  nd["num variables"] = T::ArgsCount();
}
template <typename T, std::enable_if_t<IsNamedFunction<T>::value, bool>>
void convert<YAML>::decode(const GenericBC<T, 1> &bc, ::YAML::Node &nd,
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
void convert<YAML>::decode(const GenericBC<T, 2> &bc, ::YAML::Node &nd,
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
          std::enable_if_t<IsFunctionBC<R>::value && (R::dim == 0), bool>>
R convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  return R{ExprtkFunction<R::type::ArgsCount()>{nd.as<std::string>()}};
}
template <typename R,
          std::enable_if_t<IsFunctionBC<R>::value && (R::dim == 1), bool>>
R convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  std::vector<typename R::type> functions;
  for (const auto &v : nd) {
    functions.emplace_back(
        ExprtkFunction<R::type::ArgsCount()>{v.as<std::string>()});
  }
  return R{std::move(functions)};
}
template <typename R,
          std::enable_if_t<IsFunctionBC<R>::value && (R::dim == 2), bool>>
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

} // namespace bc

#endif
