#ifndef BC_BACKEND_YAML_CPP_CONVERT_H__
#define BC_BACKEND_YAML_CPP_CONVERT_H__
#include "bcBackendYamlCpp.h"
#include <yaml-cpp/yaml.h>
//#include "bcConvert.h"
#include "bcBC.h"
#include <exception>

// conversions from yaml to/from the BC datatypes
namespace YAML {
template <> struct convert<bc::BoolBC> {
  static Node encode(const bc::BoolBC &rhs) {
    return convert<bool>::encode(rhs);
  }
  static bool decode(const Node &node, bc::BoolBC &rhs) {
    return convert<bool>::decode(node, rhs);
  }
};
template <> struct convert<bc::ScalarBC> {
  static Node encode(const bc::ScalarBC &rhs) {
    return convert<double>::encode(rhs);
  }
  static bool decode(const Node &node, bc::ScalarBC &rhs) {
    return convert<double>::decode(node, rhs);
  }
};
template <> struct convert<bc::StringBC> {
  static Node encode(const bc::StringBC &rhs) {
    return convert<std::string>::encode(rhs);
  }
  static bool decode(const Node &node, bc::StringBC &rhs) {
    return convert<std::string>::decode(node, rhs);
  }
};
} // namespace YAML

namespace bc {
// Is there a better way to get this compile time
// type map defined for the convert function to use?
// Granted for YAML it is easy because everything goes
// to a node.
template <typename T> struct BackendTypeMap<T, YAML> {
  using type = ::YAML::Node;
};
/* Example with concrete types. Left here as an example of how you can
 * set the types for a backend which uses different output types for
 * each boundary condition/node type
 *template <> struct BackendTypeMap<BoolBC,YAML>{using type = ::YAML::Node;};
 *template <> struct BackendTypeMap<MatrixBC,YAML>{using type = ::YAML::Node;};
 *template <> struct BackendTypeMap<ScalarBC,YAML>{using type = ::YAML::Node;};
 *template <> struct BackendTypeMap<IntBC,YAML>{using type = ::YAML::Node;};
 *template <> struct BackendTypeMap<StringBC,YAML>{using type = ::YAML::Node;};
 *template <> struct BackendTypeMap<VectorBC,YAML>{using type = ::YAML::Node;};
 */

// These are the default conversions for a ::YAML::Node, so
// we leave the backend as generic, however if you wanted to
// create a new backend that also writes to ::YAML::Nodes,
// all you need to do is specialize the conversion functions
// that you want to have modified behavior.
template <typename Backend> struct convert<BoolBC, ::YAML::Node, Backend> {
  static BoolBC encode(const ::YAML::Node &nd) { return BoolBC(nd.as<bool>()); }
  static ::YAML::Node decode(const BoolBC &bc) {
    ::YAML::Node nd;
    // nd["name"] = bc.GetName();
    nd["type"] = "bool";
    nd["value"] = bc.data_;
    return nd;
  }
};
template <typename Backend> struct convert<MatrixBC, ::YAML::Node, Backend> {
  static MatrixBC encode(const ::YAML::Node &nd) {
    std::vector<std::vector<ScalarType>> matrix;
    for (const auto &row : nd) {
      matrix.push_back(row.as<std::vector<ScalarType>>());
    }
    return MatrixBC(matrix);
  }
  static ::YAML::Node decode(const MatrixBC &bc) {
    ::YAML::Node nd;
    return nd;
  }
};
template <typename Backend> struct convert<ScalarBC, ::YAML::Node, Backend> {
  static ScalarBC encode(const ::YAML::Node &nd) {
    return ScalarBC(nd.as<ScalarType>());
  }
  static ::YAML::Node decode(const ScalarBC &bc) {
    ::YAML::Node nd;
    return nd;
  }
};
template <typename Backend> struct convert<IntBC, ::YAML::Node, Backend> {
  static IntBC encode(const ::YAML::Node &nd) {
    return IntBC(nd.as<OrdinalType>());
  }
  static ::YAML::Node decode(const IntBC &bc) {
    ::YAML::Node nd;
    return nd;
  }
};
template <typename Backend> struct convert<StringBC, ::YAML::Node, Backend> {
  static StringBC encode(const ::YAML::Node &nd) {
    return StringBC(nd.as<std::string>());
  }
  static ::YAML::Node decode(const StringBC &bc) {
    ::YAML::Node nd;
    return nd;
  }
};
template <typename Backend> struct convert<VectorBC, ::YAML::Node, Backend> {
  static VectorBC encode(const ::YAML::Node &nd) {
    return VectorBC(nd.as<std::vector<ScalarType>>());
  }
  static ::YAML::Node decode(const VectorBC &bc) {
    ::YAML::Node nd;
    return nd;
  }
};
} // namespace bc

#endif
