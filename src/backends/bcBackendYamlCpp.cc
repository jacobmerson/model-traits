#include "bcBackendYamlCpp.h"
#include "bcBC.h"
#include <fmt/ostream.h>
// clang-format off
// it's currently important that bcBackendYamlCppConvert.h
// gets loaded after yaml-cpp
#include <yaml-cpp/yaml.h>
#include "bcBackendYamlCppConvert.h"
// clang-format on
#include "bcModelTraits.h"

namespace mt {

template <typename T, int dim>
static void AddExpression(const ::YAML::Node &bc, CategoryNode *parent_node,
                          std::string &&name, GeometrySet<> &&geometry_set) {
  auto num_variables = bc["num variables"].as<int>();
  switch (num_variables) {
  case 1:
    parent_node->AddBoundaryCondition(
        std::move(name), std::move(geometry_set),
        FunctionBC<T, 1, dim>::template from<YAML>(bc["value"]));
    break;
  case 2:
    parent_node->AddBoundaryCondition(
        std::move(name), std::move(geometry_set),
        FunctionBC<T, 2, dim>::template from<YAML>(bc["value"]));
    break;
  case 3:
    parent_node->AddBoundaryCondition(
        std::move(name), std::move(geometry_set),
        FunctionBC<T, 3, dim>::template from<YAML>(bc["value"]));
    break;
  case 4:
    parent_node->AddBoundaryCondition(
        std::move(name), std::move(geometry_set),
        FunctionBC<T, 4, dim>::template from<YAML>(bc["value"]));
    break;
    // in the default case, the boundary condition is added as a dynamic type
  default:
    throw std::runtime_error(
        "Equations are only implemented with up to 4 arguments");
  }
}

static void ParseBoundaryConditions(const ::YAML::Node &yaml_node,
                                    CategoryNode *parent_node) {
  for (auto &&bc : yaml_node) {
    auto name = bc["name"].as<std::string>();
    auto type = bc["type"].as<std::string>();
    auto geometry = bc["geometry"].as<std::vector<int>>();
    auto geometry_set = GeometrySet<>(geometry.begin(), geometry.end());
    if (type == "scalar") {
      auto value = ScalarBC::from<YAML>(bc["value"]);
      parent_node->AddBoundaryCondition(name, std::move(geometry_set),
                                        std::move(value));
    } else if (type == "bool") {
      auto value = BoolBC::from<YAML>(bc["value"]);
      parent_node->AddBoundaryCondition(name, std::move(geometry_set),
                                        std::move(value));
    } else if (type == "int") {
      auto value = IntBC::from<YAML>(bc["value"]);
      parent_node->AddBoundaryCondition(name, std::move(geometry_set),
                                        std::move(value));
    } else if (type == "string") {
      auto value = StringBC::from<YAML>(bc["value"]);
      parent_node->AddBoundaryCondition(name, std::move(geometry_set),
                                        std::move(value));
    } else if (type == "vector") {
      auto value = VectorBC::from<YAML>(bc["value"]);
      parent_node->AddBoundaryCondition(name, std::move(geometry_set),
                                        std::move(value));
    } else if (type == "matrix") {
      auto value = MatrixBC::from<YAML>(bc["value"]);
      parent_node->AddBoundaryCondition(name, std::move(geometry_set),
                                        std::move(value));
    } else if (type == "expression") {
      AddExpression<ScalarType, 0>(bc, parent_node, std::move(name),
                                   std::move(geometry_set));
    } else if (type == "vector expression") {
      AddExpression<ScalarType, 1>(bc, parent_node, std::move(name),
                                   std::move(geometry_set));
    } else if (type == "matrix expression") {
      AddExpression<ScalarType, 2>(bc, parent_node, std::move(name),
                                   std::move(geometry_set));
    } else {
      throw std::runtime_error(
          fmt::format("Boundary condition type {} not implemented.", type));
    }
  }
}

static void ParseCaseHelper(const ::YAML::Node &yaml_node,
                            CategoryNode *parent_node) {
  for (auto &&c : yaml_node) {
    auto key = c.first.as<std::string>();
    // if(c.first.as<std::string>() == "boundary conditions")
    if (key == "boundary conditions") {
      ParseBoundaryConditions(c.second, parent_node);
    } else {
      ParseCaseHelper(c.second, parent_node->AddCategory(key));
    }
  }
}

static void ParseCase(const ::YAML::Node &yaml_case,
                      ModelTraits *model_traits) {
  auto model_case = model_traits->AddCase(yaml_case["name"].as<std::string>());
  auto &case_traits = yaml_case["model traits"];
  auto &geometry_type = case_traits["geometry_type"];
  if (geometry_type && geometry_type.as<std::string>() != "int") {
    throw std::runtime_error("only integer mt types are currently supported");
  }
  ParseCaseHelper(case_traits, model_case);
}

std::unique_ptr<ModelTraits> LoadFromYamlNode(const ::YAML::Node &base_node) {
  auto model_traits =
      std::make_unique<ModelTraits>(base_node["name"].as<std::string>());
  for (auto &cs : base_node["cases"]) {
    ParseCase(cs, model_traits.get());
  }
  return model_traits;
}

template <>
std::unique_ptr<ModelTraits> ReadFromStream(std::istream &stream, YAML *) {
  auto node = ::YAML::Load(stream);
  return LoadFromYamlNode(node["model traits"]);
}

template <>
void WriteToStream(const ModelTraits *model_traits, std::ostream &stream,
                   YAML *) {
  if (model_traits == nullptr) {
    return;
  }
  ::YAML::Emitter e;

  auto n = model_traits->to<YAML>();
  // auto cs = model_traits->GetCase("case 1");
  // auto n = cs->to<YAML>();
  e << n;
  fmt::print(stream, "{}\n", e.c_str());
}

} // namespace mt
