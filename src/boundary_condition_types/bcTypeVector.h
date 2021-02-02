#ifndef BC_TYPE_VECTOR_H__
#define BC_TYPE_VECTOR_H__
#include "bcTypedefs.h"
#include <vector>
namespace bc {
class VectorBC {
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
