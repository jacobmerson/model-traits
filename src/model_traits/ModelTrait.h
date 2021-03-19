#ifndef BC_BC_H__
#define BC_BC_H__

#include "Convert.h"
#include "FunctionHelper.h"
#include "NamedFunction.h"
#include "TypeTraits.h"
#include "Typedefs.h"
#include <functional>
#include <stdexcept>
#include <vector>

namespace mt {

template <typename, int = 0> class GenericMT;
using BoolMT = GenericMT<bool>;
using ScalarMT = GenericMT<ScalarType>;
using StringMT = GenericMT<std::string>;
using IntMT = GenericMT<OrdinalType>;
using MatrixMT = GenericMT<ScalarType, 2>;
using VectorMT = GenericMT<ScalarType, 1>;

// typedef to define the function boundary condition types
// T is the return type of the function, NARG is the number of
// arguments that the function takes (ScalarType), and dim is the
// dimension of the boundary condition:
// dim=0: Scalar,Int, bool
// dim=1: vector
// dim=2: matrix
template <typename T, int NARG, int dim = 0>
using FunctionMT =
    GenericMT<NamedFunction<FunctionT<T, ScalarType, NARG>>, dim>;

// convenience typedefs for function types
template <int NARG> using BoolFunctionMT = FunctionMT<bool, NARG, 0>;
template <int NARG> using ScalarFunctionMT = FunctionMT<ScalarType, NARG, 0>;
template <int NARG> using StringFunctionMT = FunctionMT<std::string, NARG, 0>;
template <int NARG> using IntFunctionMT = FunctionMT<OrdinalType, NARG, 0>;
template <int NARG> using MatrixFunctionMT = FunctionMT<ScalarType, NARG, 2>;
template <int NARG> using VectorFunctionMT = FunctionMT<ScalarType, NARG, 1>;

// TODO: move the visitor to an independent header
// TODO: decide if the visit functions should have a default empty
// implementation
struct MTVisitor {
  virtual void visit(BoolMT &) = 0;
  virtual void visit(MatrixMT &) = 0;
  virtual void visit(ScalarMT &) = 0;
  virtual void visit(IntMT &) = 0;
  virtual void visit(StringMT &) = 0;
  virtual void visit(VectorMT &) = 0;
  // equation types
  // 4 parameters (space and time)
  virtual void visit(BoolFunctionMT<4> &){};
  virtual void visit(ScalarFunctionMT<4> &){};
  virtual void visit(StringFunctionMT<4> &){};
  virtual void visit(IntFunctionMT<4> &){};
  virtual void visit(MatrixFunctionMT<4> &){};
  virtual void visit(VectorFunctionMT<4> &){};
  // 3 parameters (3D space)
  virtual void visit(BoolFunctionMT<3> &){};
  virtual void visit(ScalarFunctionMT<3> &){};
  virtual void visit(StringFunctionMT<3> &){};
  virtual void visit(IntFunctionMT<3> &){};
  virtual void visit(MatrixFunctionMT<3> &){};
  virtual void visit(VectorFunctionMT<3> &){};
  // 2 parameters (2D space)
  virtual void visit(BoolFunctionMT<2> &){};
  virtual void visit(ScalarFunctionMT<2> &){};
  virtual void visit(StringFunctionMT<2> &){};
  virtual void visit(IntFunctionMT<2> &){};
  virtual void visit(MatrixFunctionMT<2> &){};
  virtual void visit(VectorFunctionMT<2> &){};
  // 1 parameters (time)
  virtual void visit(BoolFunctionMT<1> &){};
  virtual void visit(ScalarFunctionMT<1> &){};
  virtual void visit(StringFunctionMT<1> &){};
  virtual void visit(IntFunctionMT<1> &){};
  virtual void visit(MatrixFunctionMT<1> &){};
  virtual void visit(VectorFunctionMT<1> &){};
  virtual ~MTVisitor() = default;
};

class IModelTrait {
public:
  virtual void accept(MTVisitor &v) = 0;
  IModelTrait() = default;
  IModelTrait(const IModelTrait &) = default;
  IModelTrait(IModelTrait &&) = default;
  IModelTrait &operator=(const IModelTrait &) = default;
  IModelTrait &operator=(IModelTrait &&) = default;
  virtual ~IModelTrait() = default;
};

template <typename T>
class GenericMT<T, 2> : public IModelTrait,
                        public Convertible<GenericMT<T, 2>> {
public:
  using type = T;
  static constexpr int dim = 2;
  GenericMT() noexcept = default;
  GenericMT(OrdinalType nrows, OrdinalType ncols) : data_(nrows * ncols) {}
  // not noexcept because copying d in the parameter can throw
  explicit GenericMT(std::vector<std::vector<T>> d) {
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
  void accept(MTVisitor &v) final { v.visit(*this); }

  OrdinalType nrows() const { return data_.size() / ncols_; }
  OrdinalType ncols() const { return ncols_; }

private:
  std::vector<T> data_;
  OrdinalType ncols_;
};

template <typename R, typename... Args>
class GenericMT<NamedFunction<R(Args...)>, 2>
    : public IModelTrait,
      public Convertible<GenericMT<NamedFunction<R(Args...)>, 2>> {
public:
  using type = NamedFunction<R(Args...)>;
  static constexpr int dim = 2;
  GenericMT() noexcept = default;
  GenericMT(OrdinalType nrows, OrdinalType ncols) : data_(nrows * ncols) {}
  // not noexcept because copying d in the parameter can throw
  explicit GenericMT(std::vector<std::vector<type>> d) {
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
  const type &operator()(OrdinalType row, OrdinalType col) const noexcept {
    return data_[row + col * ncols_];
  }
  type &operator()(OrdinalType row, OrdinalType col) noexcept {
    return data_[row + col * ncols_];
  }
  R operator()(OrdinalType row, OrdinalType col, const Args &...args) {
    return data_[row + col * ncols_](args...);
  }
  R operator()(OrdinalType row, OrdinalType col, const Args &...args) const {
    return data_[row + col * ncols_](args...);
  }
  void accept(MTVisitor &v) final { v.visit(*this); }

  OrdinalType nrows() const { return data_.size() / ncols_; }
  OrdinalType ncols() const { return ncols_; }

private:
  std::vector<type> data_;
  OrdinalType ncols_;
};

template <typename T>
class GenericMT<T, 1> : public IModelTrait,
                        public Convertible<GenericMT<T, 1>> {
public:
  using type = T;
  static constexpr int dim = 1;
  GenericMT() noexcept = default;
  GenericMT(OrdinalType size) : data_(size){};
  // not noexcept because copying d in the parameter can throw
  explicit GenericMT(const std::vector<T> &d) : data_(d){};
  explicit GenericMT(std::vector<T> &&d) noexcept : data_(std::move(d)){};
  T &operator()(OrdinalType i) noexcept { return data_[i]; }
  const T &operator()(OrdinalType i) const noexcept { return data_[i]; }
  auto size() const { return data_.size(); }
  void accept(MTVisitor &v) final { v.visit(*this); }

private:
  std::vector<T> data_;
};

template <typename R, typename... Args>
class GenericMT<NamedFunction<R(Args...)>, 1>
    : public IModelTrait,
      public Convertible<GenericMT<NamedFunction<R(Args...)>, 1>> {
public:
  using type = NamedFunction<R(Args...)>;
  static constexpr int dim = 1;
  GenericMT() noexcept = default;
  GenericMT(OrdinalType size) : data_(size){};
  // not noexcept because copying d in the parameter can throw
  explicit GenericMT(const std::vector<type> &d) : data_(d){};
  explicit GenericMT(std::vector<type> &&d) noexcept : data_(std::move(d)){};
  type &operator()(OrdinalType i) { return data_[i]; }
  const type &operator()(OrdinalType i) const { return data_[i]; }
  R operator()(OrdinalType i, const Args &...args) noexcept {
    return data_[i](args...);
  }
  R operator()(OrdinalType i, const Args &...args) const noexcept {
    return data_[i](args...);
  }
  auto size() const { return data_.size(); }
  void accept(MTVisitor &v) final { v.visit(*this); }

private:
  std::vector<type> data_;
};

template <typename T>
class GenericMT<T, 0> : public IModelTrait,
                        public Convertible<GenericMT<T, 0>> {
public:
  using type = T;
  static constexpr int dim = 0;
  GenericMT() = default;
  GenericMT(const T &d) noexcept : data_(d){};
  GenericMT(T &&d) noexcept : data_(std::move(d)){};
  T &operator()() noexcept { return data_; }
  const T &operator()() const noexcept { return data_; }

  operator T &() noexcept { return data_; }
  operator const T &() const noexcept { return data_; }
  void accept(MTVisitor &v) final { v.visit(*this); }
  const T &GetData() const noexcept { return data_; }
  T &GetData() noexcept { return data_; }

private:
  T data_;
};

template <typename R, typename... Args>
class GenericMT<NamedFunction<R(Args...)>, 0>
    : public IModelTrait,
      public Convertible<GenericMT<NamedFunction<R(Args...)>, 0>> {
public:
  using type = NamedFunction<R(Args...)>;
  static constexpr int dim = 0;
  GenericMT() = default;
  GenericMT(const type &d) noexcept : data_(d){};
  GenericMT(type &&d) noexcept : data_(std::move(d)){};
  R operator()(const Args &...args) { return data_(args...); }
  R operator()(const Args &...args) const { return data_(args...); }

  operator type &() noexcept { return data_; }
  operator const type &() const noexcept { return data_; }
  void accept(MTVisitor &v) final { v.visit(*this); }
  const type &GetData() const noexcept { return data_; }
  type &GetData() noexcept { return data_; }

private:
  type data_;
};

template <typename T> struct IsGenericMT : public std::false_type {};
template <typename T, int dim>
struct IsGenericMT<GenericMT<T, dim>> : public std::true_type {};
static_assert(IsGenericMT<GenericMT<int, 0>>::value,
              "IsGeneric MT should return true for GenericMT types");
static_assert(IsGenericMT<FunctionMT<int, 2, 1>>::value,
              "IsGeneric MT should return true for GenericMT types");
static_assert(
    !IsGenericMT<int>::value,
    "IsGeneric MT should return false for types that aren't a GenericMT");

template <typename T, typename = void>
struct IsFunctionMT : public std::false_type {};
template <typename T>
struct IsFunctionMT<
    T, VoidT<std::enable_if_t<IsGenericMT<T>::value &&
                              IsNamedFunction<typename T::type>::value>>>
    : public std::true_type {};

static_assert(IsFunctionMT<FunctionMT<double, 1, 0>>::value,
              "IsFunctionMT should return true for FunctionMT types");
static_assert(!IsFunctionMT<int>::value,
              "IsFunctionMT should return false for non FunctionMT types");

template <typename ModelTrait> const ModelTrait *MTCast(const IModelTrait *mt) {
  return dynamic_cast<const ModelTrait *>(mt);
}
} // namespace mt

#endif
