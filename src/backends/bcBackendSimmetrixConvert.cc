#include "bcBackendSimmetrixConvert.h"
#include "bcBC.h"
#include "bcBackendSimmetrix.h"
#include "bcConvert.h"
#include "bcSimWrapper.h"
#include "bcTypedefs.h"
#include <SimAttribute.h>

namespace bc {
template <>
IntBC convert<SIMMETRIX>::encode(const pAttInfoInt &att, SIMMETRIX *) {
  return IntBC(AttInfoInt_value(att));
}
template <>
MatrixBC convert<SIMMETRIX>::encode(const pAttInfoTensor2 &att, SIMMETRIX *) {
  int size = AttInfoTensor2_dimension(att);
  std::vector<std::vector<double>> values(size, std::vector<double>(size));
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      values[i][j] = AttInfoTensor2_value(att, i, j);
    }
  }
  return MatrixBC(values);
}
template <>
ScalarBC convert<SIMMETRIX>::encode(const pAttInfoDouble &att, SIMMETRIX *) {
  return ScalarBC(AttInfoDouble_value(att));
}
template <>
ScalarBC convert<SIMMETRIX>::encode(const pAttInfoTensor0 &att, SIMMETRIX *) {
  return ScalarBC(AttInfoTensor0_value(att));
}
template <>
StringBC convert<SIMMETRIX>::encode(const pAttInfoString &att, SIMMETRIX *) {
  SimString s = AttInfoString_value(att);
  return StringBC(std::string(s));
}

template <>
VectorBC convert<SIMMETRIX>::encode(const pAttInfoTensor1 &att, SIMMETRIX *) {
  int size = AttInfoTensor1_dimension(att);
  std::vector<double> values;
  values.reserve(size);
  for (int i = 0; i < size; ++i) {
    values.push_back(AttInfoTensor1_value(att, i));
  }
  return VectorBC(std::move(values));
}

} // namespace bc