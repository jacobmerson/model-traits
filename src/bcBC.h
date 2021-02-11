#ifndef BC_BC_H__
#define BC_BC_H__

#include "bcTypedefs.h"
#include <vector>

namespace bc {
struct IBoundaryCondition {};

struct BoolBC : public IBoundaryCondition {
  BoolBC() = default;
  BoolBC(bool d) noexcept : data(d){};
  operator bool() const noexcept { return data; }
  operator bool &() noexcept { return data; }
  bool data;
};
class MatrixBC : public IBoundaryCondition {
public:
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

private:
  std::vector<std::vector<ScalarType>> data_;
};
struct ScalarBC : public IBoundaryCondition {
  ScalarBC() = default;
  ScalarBC(ScalarType d) noexcept : data(d){};
  operator double() const noexcept { return data; }
  operator double &() noexcept { return data; }
  operator const double &() const noexcept { return data; }
  ScalarType data;
};
struct StringBC : public IBoundaryCondition {
  StringBC() = default;
  // not noexcept because copying d in the parameter can throw
  StringBC(std::string d) : data(std::move(d)){};
  StringBC(std::string &&d) noexcept : data(std::move(d)){};
  operator std::string &() noexcept { return data; }
  operator const std::string &() const noexcept { return data; }
  std::string data;
};
class VectorBC : public IBoundaryCondition {
public:
  VectorBC() = default;
  // not noexcept because copying d in the parameter can throw
  VectorBC(std::vector<ScalarType> d) : data_(std::move(d)){};
  VectorBC(std::vector<ScalarType> &&d) noexcept : data_(std::move(d)){};
  ScalarType &operator()(std::size_t i) noexcept { return data_[i]; }
  const ScalarType &operator()(std::size_t i) const noexcept {
    return data_[i];
  }

private:
  std::vector<ScalarType> data_;
};

} // namespace bc

#endif
