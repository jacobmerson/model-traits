#ifndef BC_BC_H__
#define BC_BC_H__

#include "bcConvert.h"
#include "bcTypedefs.h"
#include <vector>

namespace bc {
template <typename> struct GenericBC;
using BoolBC = GenericBC<bool>;
using ScalarBC = GenericBC<ScalarType>;
using StringBC = GenericBC<std::string>;
using IntBC = GenericBC<OrdinalType>;
struct MatrixBC;
struct VectorBC;

struct BCVisitor {
  virtual void visit(BoolBC &) = 0;
  virtual void visit(MatrixBC &) = 0;
  virtual void visit(ScalarBC &) = 0;
  virtual void visit(IntBC &) = 0;
  virtual void visit(StringBC &) = 0;
  virtual void visit(VectorBC &) = 0;
  virtual ~BCVisitor() = default;
};

struct IBoundaryCondition {
  virtual void accept(BCVisitor &v) = 0;
  IBoundaryCondition() = default;
  IBoundaryCondition(const IBoundaryCondition &) = default;
  IBoundaryCondition(IBoundaryCondition &&) = default;
  IBoundaryCondition &operator=(const IBoundaryCondition &) = default;
  IBoundaryCondition &operator=(IBoundaryCondition &&) = default;
  virtual ~IBoundaryCondition() = default;
};

struct MatrixBC : public IBoundaryCondition, public Convertible<MatrixBC> {
  MatrixBC() = default;
  // not noexcept because copying d in the parameter can throw
  MatrixBC(std::vector<std::vector<ScalarType>> d) : data_(std::move(d)){};
  MatrixBC(std::vector<std::vector<ScalarType>> &&d) noexcept
      : data_(std::move(d)){};
  const ScalarType &operator()(std::size_t row,
                               std::size_t col) const noexcept {
    return data_[row][col];
  }
  ScalarType &operator()(std::size_t row, std::size_t col) noexcept {
    return data_[row][col];
  }
  void accept(BCVisitor &v) final { v.visit(*this); }

  std::vector<std::vector<ScalarType>> data_;
};
struct VectorBC : public IBoundaryCondition, public Convertible<VectorBC> {
  VectorBC() = default;
  // not noexcept because copying d in the parameter can throw
  VectorBC(const std::vector<ScalarType> &d) : data_(d){};
  VectorBC(std::vector<ScalarType> &&d) noexcept : data_(std::move(d)){};
  ScalarType &operator()(std::size_t i) noexcept { return data_[i]; }
  const ScalarType &operator()(std::size_t i) const noexcept {
    return data_[i];
  }
  void accept(BCVisitor &v) final { v.visit(*this); }
  std::vector<ScalarType> data_;
};

template <typename T>
struct GenericBC : public IBoundaryCondition, public Convertible<GenericBC<T>> {
  GenericBC() = default;
  GenericBC(T d) noexcept : data_(d){};
  operator T &() noexcept { return data_; }
  operator const T &() const noexcept { return data_; }
  T data_;
  void accept(BCVisitor &v) final { v.visit(*this); }
};

} // namespace bc

#endif
