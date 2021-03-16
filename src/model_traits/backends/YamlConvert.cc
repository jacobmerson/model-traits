#include "YamlConvert.h"
#include <cassert>
namespace mt {

class YamlExportBCVisitor : public MTVisitor {
public:
  explicit YamlExportBCVisitor(::YAML::Node &nd, YAML *backend)
      : nd_(nd), backend_(backend) {}
  void visit(BoolMT &bc) final { bc.to(nd_, backend_); };
  void visit(MatrixMT &bc) final { bc.to(nd_, backend_); };
  void visit(ScalarMT &bc) final { bc.to(nd_, backend_); };
  void visit(IntMT &bc) final { bc.to(nd_, backend_); };
  void visit(StringMT &bc) final { bc.to(nd_, backend_); };
  void visit(VectorMT &bc) final { bc.to(nd_, backend_); };
  // equations
  void visit(BoolFunctionMT<4> &bc) final { bc.to(nd_, backend_); };
  void visit(ScalarFunctionMT<4> &bc) final { bc.to(nd_, backend_); };
  void visit(StringFunctionMT<4> &bc) final { bc.to(nd_, backend_); };
  void visit(IntFunctionMT<4> &bc) final { bc.to(nd_, backend_); };
  void visit(VectorFunctionMT<4> &bc) final { bc.to(nd_, backend_); };
  void visit(MatrixFunctionMT<4> &bc) final { bc.to(nd_, backend_); };
  // 3 parameters (3D space)
  void visit(BoolFunctionMT<3> &bc) final { bc.to(nd_, backend_); };
  void visit(ScalarFunctionMT<3> &bc) final { bc.to(nd_, backend_); };
  void visit(StringFunctionMT<3> &bc) final { bc.to(nd_, backend_); };
  void visit(IntFunctionMT<3> &bc) final { bc.to(nd_, backend_); };
  void visit(MatrixFunctionMT<3> &bc) final { bc.to(nd_, backend_); };
  void visit(VectorFunctionMT<3> &bc) final { bc.to(nd_, backend_); };
  // 2 parameters (2D space)
  void visit(BoolFunctionMT<2> &bc) final { bc.to(nd_, backend_); };
  void visit(ScalarFunctionMT<2> &bc) final { bc.to(nd_, backend_); };
  void visit(StringFunctionMT<2> &bc) final { bc.to(nd_, backend_); };
  void visit(IntFunctionMT<2> &bc) final { bc.to(nd_, backend_); };
  void visit(MatrixFunctionMT<2> &bc) final { bc.to(nd_, backend_); };
  void visit(VectorFunctionMT<2> &bc) final { bc.to(nd_, backend_); };
  // 1 parameters (time)
  void visit(BoolFunctionMT<1> &bc) final { bc.to(nd_, backend_); };
  void visit(ScalarFunctionMT<1> &bc) final { bc.to(nd_, backend_); };
  void visit(StringFunctionMT<1> &bc) final { bc.to(nd_, backend_); };
  void visit(IntFunctionMT<1> &bc) final { bc.to(nd_, backend_); };
  void visit(MatrixFunctionMT<1> &bc) final { bc.to(nd_, backend_); };
  void visit(VectorFunctionMT<1> &bc) final { bc.to(nd_, backend_); };

private:
  ::YAML::Node &nd_;
  YAML *backend_;
};

class YamlExportGeomVisitor : public GeometrySetVisitor {
public:
  explicit YamlExportGeomVisitor(::YAML::Node &nd, YAML *backend)
      : nd_(nd), backend_(backend) {}
  void visit(IdGeometrySet &g) override { g.to(nd_, backend_); }
  void visit(DimGeometrySet &g) override { g.to(nd_, backend_); }

private:
  ::YAML::Node &nd_;
  YAML *backend_;
};

template <>
BoolMT convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  return BoolMT(nd.as<bool>());
}

void convert<YAML>::decode(const BoolMT &bc, ::YAML::Node &nd, YAML *backend) {
  if (backend == nullptr || backend->write_trait_type_on_named_types ||
      backend->name2type(nd["name"].as<std::string>()).empty()) {
    nd["type"] = "bool";
  }
  nd["value"] = bc();
}

template <>
MatrixMT convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  std::vector<std::vector<ScalarType>> matrix;
  for (const auto &row : nd) {
    matrix.push_back(row.as<std::vector<ScalarType>>());
  }
  return MatrixMT(matrix);
}
void convert<YAML>::decode(const MatrixMT &bc, ::YAML::Node &nd,
                           YAML *backend) {
  if (backend == nullptr || backend->write_trait_type_on_named_types ||
      backend->name2type(nd["name"].as<std::string>()).empty()) {
    nd["type"] = "matrix";
  }
  auto val = nd["value"];
  val.SetStyle(::YAML::EmitterStyle::Flow);
  for (int i = 0; i < bc.nrows(); ++i) {
    std::vector<ScalarType> row;
    row.reserve(bc.ncols());
    for (int j = 0; j < bc.ncols(); ++j) {
      row.push_back(bc(i, j));
    }
    val.push_back(row);
  }
}

template <>
ScalarMT convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  return ScalarMT(nd.as<ScalarType>());
}
void convert<YAML>::decode(const ScalarMT &bc, ::YAML::Node &nd,
                           YAML *backend) {
  if (backend == nullptr || backend->write_trait_type_on_named_types ||
      backend->name2type(nd["name"].as<std::string>()).empty()) {
    nd["type"] = "scalar";
  }
  nd["value"] = bc();
}

template <>
IntMT convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  return IntMT(nd.as<OrdinalType>());
}
void convert<YAML>::decode(const IntMT &bc, ::YAML::Node &nd, YAML *backend) {
  if (backend == nullptr || backend->write_trait_type_on_named_types ||
      backend->name2type(nd["name"].as<std::string>()).empty()) {
    nd["type"] = "int";
  }
  nd["value"] = bc();
}

template <>
StringMT convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  return StringMT(nd.as<std::string>());
}
void convert<YAML>::decode(const StringMT &bc, ::YAML::Node &nd,
                           YAML *backend) {
  if (backend == nullptr || backend->write_trait_type_on_named_types ||
      backend->name2type(nd["name"].as<std::string>()).empty()) {
    nd["type"] = "string";
  }
  nd["value"] = bc();
}

template <>
VectorMT convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  return VectorMT(nd.as<std::vector<ScalarType>>());
}

void convert<YAML>::decode(const VectorMT &bc, ::YAML::Node &nd,
                           YAML *backend) {

  if (backend == nullptr || backend->write_trait_type_on_named_types ||
      backend->name2type(nd["name"].as<std::string>()).empty()) {
    nd["type"] = "vector";
  }
  auto val = nd["value"];
  val.SetStyle(::YAML::EmitterStyle::Flow);
  for (size_t i = 0; i < bc.size(); ++i) {
    val.push_back(bc(i));
  }
  val.SetStyle(::YAML::EmitterStyle::Flow);
}

template <>
IdGeometrySet convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  auto vec = nd.as<std::vector<OrdinalType>>();
  return IdGeometrySet(vec.begin(), vec.end());
}

void convert<YAML>::decode(const IdGeometrySet &g, ::YAML::Node &nd,
                           YAML *backend) {
  auto gnd = nd["geometry"];
  if (backend == nullptr || backend->write_geometry_type_per_model_trait ||
      backend->default_geometry_type != "id") {
    nd["geometry type"] = "id";
  }
  gnd.SetStyle(::YAML::EmitterStyle::Flow);
  for (const auto &gid : g) {
    gnd.push_back(gid);
  }
}

template <>
DimGeometrySet convert<YAML>::encode(const ::YAML::Node &nd,
                                     YAML * /*unused*/) {
  std::vector<DimGeometry> vec;
  for (const auto &g : nd) {
    vec.emplace_back(g[0].as<int>(), g[1].as<int>());
  }
  return DimGeometrySet(vec.begin(), vec.end());
}
void convert<YAML>::decode(const DimGeometrySet &g, ::YAML::Node &nd,
                           YAML *backend) {
  auto gnd = nd["geometry"];
  if (backend == nullptr || backend->write_geometry_type_per_model_trait ||
      backend->default_geometry_type != "dimension id") {
    nd["geometry type"] = "dimension id";
  }
  gnd.SetStyle(::YAML::EmitterStyle::Flow);
  for (const auto &dg : g) {
    gnd.push_back(std::make_pair(dg.GetDimension(), dg.GetID()));
  }
}

void convert<YAML>::decode(const ModelTraitNode &bcn, ::YAML::Node &nd,
                           YAML *backend) {
  for (const auto &bc : bcn.GetModelTraits()) {
    ::YAML::Node local;
    local["name"] = bcn.GetName();
    YamlExportBCVisitor bc_visitor(local, backend);
    YamlExportGeomVisitor geom_visitor(local, backend);
    bc.second->accept(bc_visitor);
    bc.first->accept(geom_visitor);
    nd.push_back(local);
  }
}
void convert<YAML>::decode(const CategoryNode &cn, ::YAML::Node &nd,
                           YAML *backend) {
  assert(backend != nullptr);
  auto bcn = nd[backend->model_trait_key];
  for (const auto &bc : cn.GetModelTraitNodes()) {
    bc.to(bcn, backend);
  }
  for (const auto &cat : cn.GetCategoryNodes()) {
    nd[cat.GetName()] = cat.to(backend);
  }
}

void convert<YAML>::decode(const ModelTraits &mt, ::YAML::Node &nd,
                           YAML *backend) {
  assert(backend != nullptr);
  auto mtn = nd[backend->model_traits_prefix];
  mtn["name"] = mt.GetName();
  auto cases_node = mtn["cases"];
  for (const auto &cs : mt.GetCases()) {
    ::YAML::Node local;
    local["name"] = cs.GetName();
    local["default geometry type"] = backend->default_geometry_type;
    local[backend->model_traits_prefix] = cs.to(backend);
    cases_node.push_back(local);
  }
}
} // namespace mt