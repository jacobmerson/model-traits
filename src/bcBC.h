#ifndef BC_BC_H__
#define BC_BC_H__

#include "bcConvert.h"
#include "bcFunctionHelper.h"
#include "bcNamedFunction.h"
#include "bcTypeTraits.h"
#include "bcTypedefs.h"
#include <functional>
#include <stdexcept>
#include <vector>

namespace mt {

template <typename, int = 0> class GenericBC;
using BoolBC = GenericBC<bool>;
using ScalarBC = GenericBC<ScalarType>;
using StringBC = GenericBC<std::string>;
using IntBC = GenericBC<OrdinalType>;
using MatrixBC = GenericBC<ScalarType, 2>;
using VectorBC = GenericBC<ScalarType, 1>;

// typedef to define the function boundary condition types
// T is the return type of the function, NARG is the number of
// arguments that the function takes (ScalarType), and dim is the
// dimension of the boundary condition:
// dim=0: Scalar,Int, bool
// dim=1: vector
// dim=2: matrix
template <typename T, int NARG, int dim = 0>
using FunctionBC =
    GenericBC<NamedFunction<FunctionT<T, ScalarType, NARG>>, dim>;

// convenience typedefs for function types
template <int NARG> using BoolFunctionBC = FunctionBC<bool, NARG, 0>;
template <int NARG> using ScalarFunctionBC = FunctionBC<ScalarType, NARG, 0>;
template <int NARG> using StringFunctionBC = FunctionBC<std::string, NARG, 0>;
template <int NARG> using IntFunctionBC = FunctionBC<OrdinalType, NARG, 0>;
template <int NARG> using MatrixFunctionBC = FunctionBC<ScalarType, NARG, 2>;
template <int NARG> using VectorFunctionBC = FunctionBC<ScalarType, NARG, 1>;

// TODO: move the visitor to an independent header
// TODO: decide if the visit functions should have a default empty
// implementation
struct BCVisitor {
  virtual void visit(BoolBC &) = 0;
  virtual void visit(MatrixBC &) = 0;
  virtual void visit(ScalarBC &) = 0;
  virtual void visit(IntBC &) = 0;
  virtual void visit(StringBC &) = 0;
  virtual void visit(VectorBC &) = 0;
  // equation types
  // 4 parameters (space and time)
  virtual void visit(BoolFunctionBC<4> &){};
  virtual void visit(ScalarFunctionBC<4> &){};
  virtual void visit(StringFunctionBC<4> &){};
  virtual void visit(IntFunctionBC<4> &){};
  virtual void visit(MatrixFunctionBC<4> &){};
  virtual void visit(VectorFunctionBC<4> &){};
  // 3 parameters (3D space)
  virtual void visit(BoolFunctionBC<3> &){};
  virtual void visit(ScalarFunctionBC<3> &){};
  virtual void visit(StringFunctionBC<3> &){};
  virtual void visit(IntFunctionBC<3> &){};
  virtual void visit(MatrixFunctionBC<3> &){};
  virtual void visit(VectorFunctionBC<3> &){};
  // 2 parameters (2D space)
  virtual void visit(BoolFunctionBC<2> &){};
  virtual void visit(ScalarFunctionBC<2> &){};
  virtual void visit(StringFunctionBC<2> &){};
  virtual void visit(IntFunctionBC<2> &){};
  virtual void visit(MatrixFunctionBC<2> &){};
  virtual void visit(VectorFunctionBC<2> &){};
  // 1 parameters (time)
  virtual void visit(BoolFunctionBC<1> &){};
  virtual void visit(ScalarFunctionBC<1> &){};
  virtual void visit(StringFunctionBC<1> &){};
  virtual void visit(IntFunctionBC<1> &){};
  virtual void visit(MatrixFunctionBC<1> &){};
  virtual void visit(VectorFunctionBC<1> &){};
  virtual ~BCVisitor() = default;
};

class IBoundaryCondition {
public:
  virtual void accept(BCVisitor &v) = 0;
  IBoundaryCondition() = default;
  IBoundaryCondition(const IBoundaryCondition &) = default;
  IBoundaryCondition(IBoundaryCondition &&) = default;
  IBoundaryCondition &operator=(const IBoundaryCondition &) = default;
  IBoundaryCondition &operator=(IBoundaryCondition &&) = default;
  virtual ~IBoundaryCondition() = default;
};

template <typename T>
class GenericBC<T, 2> : public IBoundaryCondition,
                        public Convertible<GenericBC<T, 2>> {
public:
  using type = T;
  static constexpr int dim = 2;
  GenericBC() noexcept = default;
  GenericBC(OrdinalType nrows, OrdinalType ncols) : data_(nrows * ncols) {}
  // not noexcept because copying d in the parameter can throw
  explicit GenericBC(std::vector<std::vector<T>> d) {
    ncols_ = d[0].size();
    data_.reserve(d.size() * ncols_);
    for (std::size_t i = 0; i < d.size(); ++i) {
      if (d[i].size() != static_cast<std::size_t>(ncols_)) {
        throw std::runtime_error(
            "All rows must have the same number of columns!");
      }
      for (OrdinalType j = 0; j < ncols_; ++j) {
        data_.push_back(std::move(d[i][j]));
      }
    }
  }
  const T &operator()(OrdinalType row, OrdinalType col) const noexcept {
    return data_[row + col * ncols_];
  }
  T &operator()(OrdinalType row, OrdinalType col) noexcept {
    return data_[row + col * ncols_];
  }
  void accept(BCVisitor &v) final { v.visit(*this); }

  OrdinalType nrows() const { return data_.size() / ncols_; }
  OrdinalType ncols() const { return ncols_; }

private:
  std::vector<T> data_;
  OrdinalType ncols_;
};

template <typename T>
class GenericBC<T, 1> : public IBoundaryCondition,
                        public Convertible<GenericBC<T, 1>> {
public:
  using type = T;
  static constexpr int dim = 1;
  GenericBC() noexcept = default;
  GenericBC(OrdinalType size) : data_(size){};
  // not noexcept because copying d in the parameter can throw
  explicit GenericBC(const std::vector<T> &d) : data_(d){};
  explicit GenericBC(std::vector<T> &&d) noexcept : data_(std::move(d)){};
  T &operator()(OrdinalType i) noexcept { return data_[i]; }
  const T &operator()(OrdinalType i) const noexcept { return data_[i]; }
  auto size() const { return data_.size(); }
  void accept(BCVisitor &v) final { v.visit(*this); }

private:
  std::vector<T> data_;
};

template <typename T>
class GenericBC<T, 0> : public IBoundaryCondition,
                        public Convertible<GenericBC<T>> {
public:
  using type = T;
  static constexpr int dim = 0;
  GenericBC() = default;
  GenericBC(const T &d) noexcept : data_(d){};
  GenericBC(T &&d) noexcept : data_(std::move(d)){};
  T &operator()() noexcept { return data_; }
  const T &operator()() const noexcept { return data_; }

  operator T &() noexcept { return data_; }
  operator const T &() const noexcept { return data_; }
  void accept(BCVisitor &v) final { v.visit(*this); }
  const T &GetData() const noexcept { return data_; }
  T &GetData() noexcept { return data_; }

private:
  T data_;
};

template <typename T> struct IsGenericBC : public std::false_type {};
template <typename T, int dim>
struct IsGenericBC<GenericBC<T, dim>> : public std::true_type {};
static_assert(IsGenericBC<GenericBC<int, 0>>::value,
              "IsGeneric BC should return true for GenericBC types");
static_assert(IsGenericBC<FunctionBC<int, 2, 1>>::value,
              "IsGeneric BC should return true for GenericBC types");
static_assert(
    !IsGenericBC<int>::value,
    "IsGeneric BC should return false for types that aren't a GenericBC");

template <typename T, typename = void>
struct IsFunctionBC : public std::false_type {};
template <typename T>
struct IsFunctionBC<
    T, VoidT<std::enable_if_t<IsGenericBC<T>::value &&
                              IsNamedFunction<typename T::type>::value>>>
    : public std::true_type {};

static_assert(IsFunctionBC<FunctionBC<double, 1, 0>>::value,
              "IsFunctionBC should return true for FunctionBC types");
static_assert(!IsFunctionBC<int>::value,
              "IsFunctionBC should return false for non FunctionBC types");

} // namespace mt

#endif
