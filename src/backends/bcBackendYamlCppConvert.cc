#include "bcBackendYamlCppConvert.h"
namespace mt {

struct YamlExportBCVisitor : public MTVisitor {
  explicit YamlExportBCVisitor(::YAML::Node &nd) : nd_(nd) {}
  void visit(BoolMT &bc) final { bc.to<YAML>(nd_); };
  void visit(MatrixMT &bc) final { bc.to<YAML>(nd_); };
  void visit(ScalarMT &bc) final { bc.to<YAML>(nd_); };
  void visit(IntMT &bc) final { bc.to<YAML>(nd_); };
  void visit(StringMT &bc) final { bc.to<YAML>(nd_); };
  void visit(VectorMT &bc) final { bc.to<YAML>(nd_); };
  // equations
  void visit(BoolFunctionMT<4> &bc) final { bc.to<YAML>(nd_); };
  void visit(ScalarFunctionMT<4> &bc) final { bc.to<YAML>(nd_); };
  void visit(StringFunctionMT<4> &bc) final { bc.to<YAML>(nd_); };
  void visit(IntFunctionMT<4> &bc) final { bc.to<YAML>(nd_); };
  void visit(VectorFunctionMT<4> &bc) final { bc.to<YAML>(nd_); };
  void visit(MatrixFunctionMT<4> &bc) final { bc.to<YAML>(nd_); };
  // 3 parameters (3D space)
  void visit(BoolFunctionMT<3> &bc) final { bc.to<YAML>(nd_); };
  void visit(ScalarFunctionMT<3> &bc) final { bc.to<YAML>(nd_); };
  void visit(StringFunctionMT<3> &bc) final { bc.to<YAML>(nd_); };
  void visit(IntFunctionMT<3> &bc) final { bc.to<YAML>(nd_); };
  void visit(MatrixFunctionMT<3> &bc) final { bc.to<YAML>(nd_); };
  void visit(VectorFunctionMT<3> &bc) final { bc.to<YAML>(nd_); };
  // 2 parameters (2D space)
  void visit(BoolFunctionMT<2> &bc) final { bc.to<YAML>(nd_); };
  void visit(ScalarFunctionMT<2> &bc) final { bc.to<YAML>(nd_); };
  void visit(StringFunctionMT<2> &bc) final { bc.to<YAML>(nd_); };
  void visit(IntFunctionMT<2> &bc) final { bc.to<YAML>(nd_); };
  void visit(MatrixFunctionMT<2> &bc) final { bc.to<YAML>(nd_); };
  void visit(VectorFunctionMT<2> &bc) final { bc.to<YAML>(nd_); };
  // 1 parameters (time)
  void visit(BoolFunctionMT<1> &bc) final { bc.to<YAML>(nd_); };
  void visit(ScalarFunctionMT<1> &bc) final { bc.to<YAML>(nd_); };
  void visit(StringFunctionMT<1> &bc) final { bc.to<YAML>(nd_); };
  void visit(IntFunctionMT<1> &bc) final { bc.to<YAML>(nd_); };
  void visit(MatrixFunctionMT<1> &bc) final { bc.to<YAML>(nd_); };
  void visit(VectorFunctionMT<1> &bc) final { bc.to<YAML>(nd_); };

private:
  ::YAML::Node &nd_;
};

struct YamlExportGeomVisitor : public GeometrySetVisitor {
  explicit YamlExportGeomVisitor(::YAML::Node &nd) : nd_(nd) {}
  void visit(GeometrySet<OrdinalType, BC_VEC_WORKAROUND> &g) override {
    g.to<YAML>(nd_);
  }
  void visit(GeometrySet<DimGeometry, BC_VEC_WORKAROUND> &g) override {
    g.to<YAML>(nd_);
  }

private:
  ::YAML::Node &nd_;
};

using SetT = convert<YAML>::SetT;
using DimSetT = convert<YAML>::DimSetT;

template <>
BoolMT convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  return BoolMT(nd.as<bool>());
}

void convert<YAML>::decode(const BoolMT &bc, ::YAML::Node &nd,
                           YAML * /*unused*/) {
  nd["type"] = "bool";
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
                           YAML * /*unused*/) {
  nd["type"] = "matrix";
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
                           YAML * /*unused*/) {
  nd["type"] = "scalar";
  nd["value"] = bc();
}

template <>
IntMT convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  return IntMT(nd.as<OrdinalType>());
}
void convert<YAML>::decode(const IntMT &bc, ::YAML::Node &nd,
                           YAML * /*unused*/) {
  nd["type"] = "int";
  nd["value"] = bc();
}

template <>
StringMT convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  return StringMT(nd.as<std::string>());
}
void convert<YAML>::decode(const StringMT &bc, ::YAML::Node &nd,
                           YAML * /*unused*/) {
  nd["type"] = "string";
  nd["value"] = bc();
}

template <>
VectorMT convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  return VectorMT(nd.as<std::vector<ScalarType>>());
}

void convert<YAML>::decode(const VectorMT &bc, ::YAML::Node &nd,
                           YAML * /*unused*/) {
  nd["type"] = "vector";
  auto val = nd["value"];
  val.SetStyle(::YAML::EmitterStyle::Flow);
  for (size_t i = 0; i < bc.size(); ++i) {
    val.push_back(bc(i));
  }
  val.SetStyle(::YAML::EmitterStyle::Flow);
}

template <>
SetT convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  auto vec = nd.as<std::vector<OrdinalType>>();
  return SetT(vec.begin(), vec.end());
}

void convert<YAML>::decode(const SetT &g, ::YAML::Node &nd, YAML * /*unused*/) {
  auto gnd = nd["geometry"];
  gnd.SetStyle(::YAML::EmitterStyle::Flow);
  for (const auto &gid : g) {
    gnd.push_back(gid);
  }
}

template <>
DimSetT convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  std::vector<DimGeometry> vec;
  for (const auto &g : nd) {
    vec.emplace_back(g[0].as<int>(), g[1].as<int>());
  }
  return DimSetT(vec.begin(), vec.end());
}
void convert<YAML>::decode(const DimSetT &g, ::YAML::Node &nd,
                           YAML * /*unused*/) {
  auto gnd = nd["geometry"];
  gnd.SetStyle(::YAML::EmitterStyle::Flow);
  for (const auto &dg : g) {
    gnd.push_back(std::make_pair(dg.GetDimension(), dg.GetID()));
  }
}

void convert<YAML>::decode(const ModelTraitNode &bcn, ::YAML::Node &nd,
                           YAML * /*unused*/) {
  for (const auto &bc : bcn.GetModelTraits()) {
    ::YAML::Node local;
    local["name"] = bcn.GetName();
    YamlExportBCVisitor bc_visitor(local);
    YamlExportGeomVisitor geom_visitor(local);
    bc.second->accept(bc_visitor);
    bc.first->accept(geom_visitor);
    nd.push_back(local);
  }
}
void convert<YAML>::decode(const CategoryNode &cn, ::YAML::Node &nd,
                           YAML * /*unused*/) {
  auto bcn = nd["boundary conditions"];
  for (const auto &bc : cn.GetModelTraitNodes()) {
    bc->to<YAML>(bcn);
  }
  for (const auto &cat : cn.GetCategories()) {
    nd[cat->GetName()] = cat->to<YAML>();
  }
}
void convert<YAML>::decode(const ModelTraits &mt, ::YAML::Node &nd,
                           YAML * /*unused*/) {
  auto mtn = nd["model traits"];
  mtn["name"] = mt.GetName();
  auto cases_node = mtn["cases"];
  for (const auto &cs : mt.GetCases()) {
    ::YAML::Node local;
    local["name"] = cs->GetName();
    local["model traits"] = cs->to<YAML>();
    cases_node.push_back(local);
  }
}
} // namespace mt