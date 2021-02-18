#include "bcBackendYamlCpp.h"
#include "bcBC.h"
#include <fmt/ostream.h>
// clang-format off
// it's currently important that bcBackendYamlCppConvert.h
// get's loaded after yaml-cpp
#include <yaml-cpp/yaml.h>
#include "bcBackendYamlCppConvert.h"
// clang-format on
#include "bcModelTraits.h"

namespace bc {

void ParseBoundaryConditions(const ::YAML::Node &yaml_node,
                             CategoryNode *parent_node) {
  for (auto &&bc : yaml_node) {
    auto name = bc["name"].as<std::string>();
    auto type = bc["type"].as<std::string>();
    auto geometry = bc["geometry"].as<std::vector<int>>();
    auto geometry_set = GeometrySet<>(geometry.begin(), geometry.end());
    if (type == "scalar") {
      auto value = ScalarBC::from<YAML>(bc["value"]);
      parent_node->AddBoundaryCondition(
          std::move(name), std::move(geometry_set), std::move(value));
    } else if (type == "bool") {
      auto value = BoolBC::from<YAML>(bc["value"]);
      parent_node->AddBoundaryCondition(
          std::move(name), std::move(geometry_set), std::move(value));
    } else if (type == "int") {
      auto value = IntBC::from<YAML>(bc["value"]);
      parent_node->AddBoundaryCondition(
          std::move(name), std::move(geometry_set), std::move(value));
    } else if (type == "string") {
      auto value = StringBC::from<YAML>(bc["value"]);
      parent_node->AddBoundaryCondition(
          std::move(name), std::move(geometry_set), std::move(value));
    } else if (type == "vector") {
      auto value = VectorBC::from<YAML>(bc["value"]);
      parent_node->AddBoundaryCondition(
          std::move(name), std::move(geometry_set), std::move(value));
    } else if (type == "matrix") {
      auto value = MatrixBC::from<YAML>(bc["value"]);
      parent_node->AddBoundaryCondition(
          std::move(name), std::move(geometry_set), std::move(value));
    } else {
      throw std::runtime_error(
          fmt::format("Boundary condition type {} not implimented.", type));
    }
  }
}

void ParseCaseHelper(const ::YAML::Node &yaml_node, CategoryNode *parent_node) {
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

void ParseCase(const ::YAML::Node &yaml_case, ModelTraits *model_traits) {
  auto model_case = model_traits->AddCase(yaml_case["name"].as<std::string>());
  auto &case_traits = yaml_case["model traits"];
  auto &geometry_type = case_traits["geometry_type"];
  if (geometry_type && geometry_type.as<std::string>() != "int") {
    throw std::runtime_error("only integer bc types are currently supported");
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
std::unique_ptr<ModelTraits> ReadFromStream<YAML>(std::istream &stream) {
  auto node = ::YAML::Load(stream);
  return LoadFromYamlNode(node["model traits"]);
  // auto node = ::YAML::Load(stream);
  // return std::make_unique<ModelTraits>(ModelTraits::from<YAML>(node));
}

template <>
void WriteToStream<YAML>(const ModelTraits *model_traits,
                         std::ostream &stream) {
  ::YAML::Emitter e;

  auto n = model_traits->to<YAML>();
  // auto cs = model_traits->GetCase("case 1");
  // auto n = cs->to<YAML>();
  e << n;
  fmt::print(stream, "{}\n", e.c_str());
}

} // namespace bc
