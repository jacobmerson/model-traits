#include "YamlConvert.h"
#include "model_traits/ModelTrait.h"
#include "model_traits/ModelTraits.h"
#include "model_traits/ModelTraitsIO.h"
#include <cassert>
#include <fmt/ostream.h>
#include <yaml-cpp/yaml.h>

namespace mt {

template <typename ModelTrait>
static std::shared_ptr<IModelTrait> ModelTraitToPtr(ModelTrait &&model_trait) {
  return std::static_pointer_cast<IModelTrait>(
      std::make_shared<ModelTrait>(model_trait));
}
template <typename Geom>
static std::shared_ptr<IGeometrySet> GeomToPtr(Geom &&geometry) {
  return std::static_pointer_cast<IGeometrySet>(
      std::make_shared<Geom>(geometry));
}
template <typename T, int dim>
static std::shared_ptr<IModelTrait> GetExpressionPtr(const ::YAML::Node &bc) {
  auto num_variables = bc["num variables"].as<int>();
  switch (num_variables) {
  case 1:
    return ModelTraitToPtr(
        FunctionMT<T, 1, dim>::template from<YAML>(bc["value"]));
  case 2:
    return ModelTraitToPtr(
        FunctionMT<T, 2, dim>::template from<YAML>(bc["value"]));
  case 3:
    return ModelTraitToPtr(
        FunctionMT<T, 3, dim>::template from<YAML>(bc["value"]));
  case 4:
    return ModelTraitToPtr(
        FunctionMT<T, 3, dim>::template from<YAML>(bc["value"]));
  default:
    throw std::runtime_error(
        "Equations are only implemented with up to 4 arguments");
  }
}

static void ParseBoundaryConditions(const ::YAML::Node &yaml_node,
                                    CategoryNode *parent_node, YAML *backend) {
  assert(backend != nullptr);
  for (auto &&bc : yaml_node) {
    const auto &name_nd = bc["name"];
    if (!name_nd) {
      throw std::runtime_error(fmt::format("No name provided on Model Trait."));
    }
    auto name = name_nd.as<std::string>();
    std::string type{};
    std::string geometry_type = backend->default_geometry_type;
    if (bc["geometry type"]) {
      geometry_type = bc["geometry type"].as<std::string>();
    }
    // get the default type for a given name
    if (backend->name2type) {
      type = backend->name2type(name);
    }
    // if the type is explicitly stated in the input file override
    if (bc["type"]) {
      std::string type2 = bc["type"].as<std::string>();
      if (!type.empty() && type2 != type) {
        fmt::print(
            "WARNING: overriding the default backend type for {} ({}) with {}",
            name, type, type2);
      }
      type = std::move(type2);
    }
    if (type.empty()) {
      throw std::runtime_error(
          fmt::format("ModelTrait {} must have a type.", name));
    }
    std::shared_ptr<IModelTrait> model_trait;
    std::shared_ptr<IGeometrySet> geometry_set;
    const auto &geometry_nd = bc["geometry"];
    const auto &value_nd = bc["value"];
    if (!geometry_nd) {
      throw std::runtime_error(
          fmt::format("No geometry provided for ModelTrait {}.", name));
    }
    if (geometry_type == "id") {
      geometry_set = GeomToPtr(IdGeometrySet::from(bc["geometry"], backend));
    } else if (geometry_type == "dimension id") {
      geometry_set = GeomToPtr(DimIdGeometrySet::from(bc["geometry"], backend));
    } else {
      throw std::runtime_error(
          fmt::format("Geometry type {} on ModelTrait {} is invalid.",
                      geometry_type, name));
    }
    if (!value_nd) {
      throw std::runtime_error(
          fmt::format("No value provided for ModelTrait {}.", name));
    }
    if (type == "scalar") {
      model_trait = ModelTraitToPtr(ScalarMT::from(bc["value"], backend));
    } else if (type == "bool") {
      model_trait = ModelTraitToPtr(BoolMT::from(bc["value"], backend));
    } else if (type == "int") {
      model_trait = ModelTraitToPtr(IntMT::from(bc["value"], backend));
    } else if (type == "string") {
      model_trait = ModelTraitToPtr(StringMT::from(bc["value"], backend));
    } else if (type == "vector") {
      model_trait = ModelTraitToPtr(VectorMT::from(bc["value"], backend));
    } else if (type == "matrix") {
      model_trait = ModelTraitToPtr(MatrixMT::from(bc["value"], backend));
    } else if (type == "expression") {
      model_trait = GetExpressionPtr<ScalarType, 0>(bc);
    } else if (type == "vector expression") {
      model_trait = GetExpressionPtr<ScalarType, 1>(bc);
    } else if (type == "matrix expression") {
      model_trait = GetExpressionPtr<ScalarType, 2>(bc);
    } else if (type == "void") {
      model_trait = ModelTraitToPtr(VoidMT{});
    } else {
      throw std::runtime_error(
          fmt::format("Model trait type {} not implemented.", type));
    }
    parent_node->AddModelTrait(name, std::move(geometry_set),
                               std::move(model_trait));
  }
}

static void ParseCaseHelper(const ::YAML::Node &yaml_node,
                            CategoryNode *parent_node, YAML *backend) {
  assert(backend != nullptr);
  for (auto &&c : yaml_node) {
    auto key = c.first.as<std::string>();
    if (key == backend->model_trait_key) {
      ParseBoundaryConditions(c.second, parent_node, backend);
    } else {
      ParseCaseHelper(c.second, parent_node->AddCategory(key), backend);
    }
  }
}

static void ParseCase(const ::YAML::Node &yaml_case, ModelTraits *model_traits,
                      YAML *backend) {
  assert(backend != nullptr);
  auto *model_case = model_traits->AddCase(yaml_case["name"].as<std::string>());
  const auto &case_traits = yaml_case[backend->model_traits_prefix];

  const auto &geometry_type = case_traits["default geometry type"];
  if (geometry_type) {
    backend->default_geometry_type = geometry_type.as<std::string>();
  }
  ParseCaseHelper(case_traits, model_case, backend);
}

std::unique_ptr<ModelTraits> LoadFromYamlNode(const ::YAML::Node &base_node,
                                              YAML *backend) {
  assert(backend != nullptr);
  auto model_traits =
      std::make_unique<ModelTraits>(base_node["name"].as<std::string>());
  for (const auto &cs : base_node["cases"]) {
    ParseCase(cs, model_traits.get(), backend);
  }
  return model_traits;
}

template <>
std::unique_ptr<ModelTraits> ReadFromStream(std::istream &stream,
                                            YAML *backend) {
  auto node = ::YAML::Load(stream);
  if (backend == nullptr) {
    auto backend_ptr = std::make_unique<YAML>();
    return LoadFromYamlNode(node[backend_ptr->model_traits_prefix],
                            backend_ptr.get());
  }
  return LoadFromYamlNode(node[backend->model_traits_prefix], backend);
}

template <>
void WriteToStream(const ModelTraits *model_traits, std::ostream &stream,
                   YAML *backend) {
  if (model_traits == nullptr) {
    return;
  }
  ::YAML::Emitter e;

  if (backend == nullptr) {
    auto backend_ptr = std::make_unique<YAML>();
    auto n = model_traits->to(backend_ptr.get());
    e << n;
  } else {
    auto n = model_traits->to(backend);
    e << n;
  }
  fmt::print(stream, "{}\n", e.c_str());
}

} // namespace mt
