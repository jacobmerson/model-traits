#ifndef BC_TYPE_MATRIX_H__
#define BC_TYPE_MATRIX_H__
#include "bcTypedefs.h"
#include <vector>
namespace bc {
class MatrixBC {
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
} // namespace bc
#endif
