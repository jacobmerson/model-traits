#include "bcBackendYamlCppConvert.h"
namespace bc {

struct YamlExportBCVisitor : public BCVisitor {
  explicit YamlExportBCVisitor(::YAML::Node &nd) : nd_(nd) {}
  void visit(BoolBC &bc) final { bc.to<YAML>(nd_); };
  void visit(MatrixBC &bc) final { bc.to<YAML>(nd_); };
  void visit(ScalarBC &bc) final { bc.to<YAML>(nd_); };
  void visit(IntBC &bc) final { bc.to<YAML>(nd_); };
  void visit(StringBC &bc) final { bc.to<YAML>(nd_); };
  void visit(VectorBC &bc) final { bc.to<YAML>(nd_); };
  // equations
  void visit(BoolFunctionBC<4> &bc) final { bc.to<YAML>(nd_); };
  void visit(ScalarFunctionBC<4> &bc) final { bc.to<YAML>(nd_); };
  void visit(StringFunctionBC<4> &bc) final { bc.to<YAML>(nd_); };
  void visit(IntFunctionBC<4> &bc) final { bc.to<YAML>(nd_); };
  void visit(VectorFunctionBC<4> &bc) final { bc.to<YAML>(nd_); };
  void visit(MatrixFunctionBC<4> &bc) final { bc.to<YAML>(nd_); };
  // 3 parameters (3D space)
  void visit(BoolFunctionBC<3> &bc) final { bc.to<YAML>(nd_); };
  void visit(ScalarFunctionBC<3> &bc) final { bc.to<YAML>(nd_); };
  void visit(StringFunctionBC<3> &bc) final { bc.to<YAML>(nd_); };
  void visit(IntFunctionBC<3> &bc) final { bc.to<YAML>(nd_); };
  void visit(MatrixFunctionBC<3> &bc) final { bc.to<YAML>(nd_); };
  void visit(VectorFunctionBC<3> &bc) final { bc.to<YAML>(nd_); };
  // 2 parameters (2D space)
  void visit(BoolFunctionBC<2> &bc) final { bc.to<YAML>(nd_); };
  void visit(ScalarFunctionBC<2> &bc) final { bc.to<YAML>(nd_); };
  void visit(StringFunctionBC<2> &bc) final { bc.to<YAML>(nd_); };
  void visit(IntFunctionBC<2> &bc) final { bc.to<YAML>(nd_); };
  void visit(MatrixFunctionBC<2> &bc) final { bc.to<YAML>(nd_); };
  void visit(VectorFunctionBC<2> &bc) final { bc.to<YAML>(nd_); };
  // 1 parameters (time)
  void visit(BoolFunctionBC<1> &bc) final { bc.to<YAML>(nd_); };
  void visit(ScalarFunctionBC<1> &bc) final { bc.to<YAML>(nd_); };
  void visit(StringFunctionBC<1> &bc) final { bc.to<YAML>(nd_); };
  void visit(IntFunctionBC<1> &bc) final { bc.to<YAML>(nd_); };
  void visit(MatrixFunctionBC<1> &bc) final { bc.to<YAML>(nd_); };
  void visit(VectorFunctionBC<1> &bc) final { bc.to<YAML>(nd_); };

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
BoolBC convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  return BoolBC(nd.as<bool>());
}

void convert<YAML>::decode(const BoolBC &bc, ::YAML::Node &nd,
                           YAML * /*unused*/) {
  nd["type"] = "bool";
  nd["value"] = bc();
}

template <>
MatrixBC convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  std::vector<std::vector<ScalarType>> matrix;
  for (const auto &row : nd) {
    matrix.push_back(row.as<std::vector<ScalarType>>());
  }
  return MatrixBC(matrix);
}
void convert<YAML>::decode(const MatrixBC &bc, ::YAML::Node &nd,
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
ScalarBC convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  return ScalarBC(nd.as<ScalarType>());
}
void convert<YAML>::decode(const ScalarBC &bc, ::YAML::Node &nd,
                           YAML * /*unused*/) {
  nd["type"] = "scalar";
  nd["value"] = bc();
}

template <>
IntBC convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  return IntBC(nd.as<OrdinalType>());
}
void convert<YAML>::decode(const IntBC &bc, ::YAML::Node &nd,
                           YAML * /*unused*/) {
  nd["type"] = "int";
  nd["value"] = bc();
}

template <>
StringBC convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  return StringBC(nd.as<std::string>());
}
void convert<YAML>::decode(const StringBC &bc, ::YAML::Node &nd,
                           YAML * /*unused*/) {
  nd["type"] = "string";
  nd["value"] = bc();
}

template <>
VectorBC convert<YAML>::encode(const ::YAML::Node &nd, YAML * /*unused*/) {
  return VectorBC(nd.as<std::vector<ScalarType>>());
}

void convert<YAML>::decode(const VectorBC &bc, ::YAML::Node &nd,
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
    gnd.push_back(std::make_pair(dg.dim_, dg.GID_));
  }
}

void convert<YAML>::decode(const BCNode &bcn, ::YAML::Node &nd,
                           YAML * /*unused*/) {
  for (const auto &bc : bcn.GetBoundaryConditions()) {
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
  for (const auto &bc : cn.GetBoundaryConditions()) {
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
} // namespace bc