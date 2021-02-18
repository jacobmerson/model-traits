#ifndef BC_BACKEND_YAML_CPP_CONVERT_H__
#define BC_BACKEND_YAML_CPP_CONVERT_H__
#include "bcBackendYamlCpp.h"
#include <yaml-cpp/yaml.h>
//#include "bcConvert.h"
#include "bcBC.h"
#include <exception>

namespace bc {
// Is there a better way to get this compile time
// type map defined for the convert function to use?
// Granted for YAML it is easy because everything goes
// to a node.
template <typename T> struct BackendTypeMap<T, YAML> {
  using type = ::YAML::Node;
};

// These are the default conversions for a ::YAML::Node, so
// we leave the backend as generic, however if you wanted to
// create a new backend that also writes to ::YAML::Nodes,
// all you need to do is specialize the conversion functions
// that you want to have modified behavior.
template <typename Backend> struct convert<BoolBC, ::YAML::Node, Backend> {
  static BoolBC encode(const ::YAML::Node &nd) { return BoolBC(nd.as<bool>()); }
  static ::YAML::Node decode(const BoolBC &bc, ::YAML::Node &nd) {
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
  static ::YAML::Node decode(const MatrixBC &bc, ::YAML::Node &nd) {
    nd["type"] = "matrix";
    auto val = nd["value"];
    val.SetStyle(::YAML::EmitterStyle::Flow);
    for (const auto &row : bc.data_) {
      val.push_back(row);
    }
    return nd;
  }
};
template <typename Backend> struct convert<ScalarBC, ::YAML::Node, Backend> {
  static ScalarBC encode(const ::YAML::Node &nd) {
    return ScalarBC(nd.as<ScalarType>());
  }
  static ::YAML::Node decode(const ScalarBC &bc, ::YAML::Node &nd) {
    nd["type"] = "scalar";
    nd["value"] = bc.data_;
    return nd;
  }
};
template <typename Backend> struct convert<IntBC, ::YAML::Node, Backend> {
  static IntBC encode(const ::YAML::Node &nd) {
    return IntBC(nd.as<OrdinalType>());
  }
  static ::YAML::Node decode(const IntBC &bc, ::YAML::Node &nd) {
    nd["type"] = "int";
    nd["value"] = bc.data_;
    return nd;
  }
};
template <typename Backend> struct convert<StringBC, ::YAML::Node, Backend> {
  static StringBC encode(const ::YAML::Node &nd) {
    return StringBC(nd.as<std::string>());
  }
  static ::YAML::Node decode(const StringBC &bc, ::YAML::Node &nd) {
    nd["type"] = "string";
    nd["value"] = bc.data_;
    return nd;
  }
};
template <typename Backend> struct convert<VectorBC, ::YAML::Node, Backend> {
  static VectorBC encode(const ::YAML::Node &nd) {
    return VectorBC(nd.as<std::vector<ScalarType>>());
  }
  static ::YAML::Node decode(const VectorBC &bc, ::YAML::Node &nd) {
    nd["type"] = "vector";
    auto val = nd["value"];
    val = bc.data_;
    val.SetStyle(::YAML::EmitterStyle::Flow);
    return nd;
  }
};
// conversion functions for the geometry
template <typename Backend>
struct convert<GeometrySet<OrdinalType, BC_VEC_WORKAROUND>, ::YAML::Node,
               Backend> {
  using SetT = GeometrySet<OrdinalType, BC_VEC_WORKAROUND>;
  static SetT encode(const ::YAML::Node &nd) {
    auto vec = nd.as<std::vector<OrdinalType>>();
    return SetT(vec.begin(), vec.end());
  }
  static ::YAML::Node decode(const SetT &g, ::YAML::Node &nd) {
    auto gnd = nd["geometry"];
    gnd.SetStyle(::YAML::EmitterStyle::Flow);
    for (auto &gid : g) {
      gnd.push_back(gid);
    }
    return nd;
  }
};
template <typename Backend>
struct convert<GeometrySet<DimGeometry, BC_VEC_WORKAROUND>, ::YAML::Node,
               Backend> {
  using SetT = GeometrySet<DimGeometry, BC_VEC_WORKAROUND>;
  static SetT encode(const ::YAML::Node &nd) {
    std::vector<DimGeometry> vec;
    for (auto &g : nd) {
      vec.emplace_back(g[0], g[1]);
    }
    return SetT(vec.begin(), vec.end());
  }
  static void decode(const SetT &g, ::YAML::Node &nd) {
    auto gnd = nd["geometry"];
    gnd.SetStyle(::YAML::EmitterStyle::Flow);
    for (auto &dg : g) {
      gnd.push_back(std::make_pair(dg.dim_, dg.GID_));
    }
  }
};

// convert the "nodes"
struct YamlExportBCVisitor : public BCVisitor {
  YamlExportBCVisitor(::YAML::Node &nd) : nd_(nd) {}
  void visit(BoolBC &bc) final { bc.to<YAML>(nd_); };
  void visit(MatrixBC &bc) final { bc.to<YAML>(nd_); };
  void visit(ScalarBC &bc) final { bc.to<YAML>(nd_); };
  void visit(IntBC &bc) final { bc.to<YAML>(nd_); };
  void visit(StringBC &bc) final { bc.to<YAML>(nd_); };
  void visit(VectorBC &bc) final { bc.to<YAML>(nd_); };

  ::YAML::Node &nd_;
};

struct YamlExportGeomVisitor : public GeometrySetVisitor {
  YamlExportGeomVisitor(::YAML::Node &nd) : nd_(nd) {}
  virtual void visit(GeometrySet<OrdinalType, BC_VEC_WORKAROUND> &g) {
    g.to<YAML>(nd_);
  }
  virtual void visit(GeometrySet<DimGeometry, BC_VEC_WORKAROUND> &g) {
    g.to<YAML>(nd_);
  }
  ::YAML::Node &nd_;
};

template <typename Backend> struct convert<BCNode, ::YAML::Node, Backend> {
  // static BCNode encode(const ::YAML::Node &nd) { return BCNode{}; }
  static ::YAML::Node decode(const BCNode &bcn, ::YAML::Node &nd) {
    for (auto &bc : bcn.GetBoundaryConditions()) {
      ::YAML::Node local;
      local["name"] = bcn.GetName();
      YamlExportBCVisitor bc_visitor(local);
      YamlExportGeomVisitor geom_visitor(local);
      bc.second->accept(bc_visitor);
      bc.first->accept(geom_visitor);
      nd.push_back(local);
    }
    return nd;
  }
};

template <typename Backend>
struct convert<CategoryNode, ::YAML::Node, Backend> {
  // static CategoryNode encode(const ::YAML::Node &nd) { return CategoryNode{};
  // }
  static ::YAML::Node decode(const CategoryNode &cn, ::YAML::Node &nd) {
    auto bcn = nd["boundary conditions"];
    for (const auto &bc : cn.GetBoundaryConditions()) {
      bc->to<YAML>(bcn);
    }
    for (const auto &cat : cn.GetCategories()) {
      nd[cat->GetName()] = cat->to<YAML>();
    }
    return nd;
  }
};
template <typename Backend> struct convert<ModelTraits, ::YAML::Node, Backend> {
  static ::YAML::Node decode(const ModelTraits &mt, ::YAML::Node &nd) {
    auto mtn = nd["model traits"];
    mtn["name"] = mt.GetName();
    auto casesn = mtn["cases"];
    for (const auto &cs : mt.GetCases()) {
      ::YAML::Node local;
      local["name"] = cs->GetName();
      local["model traits"] = cs->to<YAML>();
      casesn.push_back(local);
    }
    return nd;
  }
};

} // namespace bc

#endif
