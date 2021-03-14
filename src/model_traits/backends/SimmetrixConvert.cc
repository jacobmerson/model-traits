#include "SimmetrixConvert.h"
#include "Simmetrix.h"
#include "SimmetrixWrapper.h"
#include "model_traits/Convert.h"
#include "model_traits/ModelTrait.h"
#include "model_traits/Typedefs.h"
#include <SimAttribute.h>

namespace mt {
template <> IntMT convert<SIMMETRIX>::encode(pAttInfo att, SIMMETRIX *) {
  auto int_att = static_cast<pAttInfoInt>(att);
  if (AttInfoInt_isExpression(int_att)) {
    SimString expression = AttInfoInt_expression(int_att);
    return IntMT(std::stoi(std::string(expression)));
  } else {
    return IntMT(AttInfoInt_value(int_att));
  }
}
template <> MatrixMT convert<SIMMETRIX>::encode(pAttInfo att, SIMMETRIX *) {
  auto tensor_att = static_cast<pAttInfoTensor2>(att);
  int size = AttInfoTensor2_dimension(tensor_att);
  std::vector<std::vector<double>> values(size, std::vector<double>(size));
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      // only get here if all of the components of the expression have no
      // variables
      if (AttInfoTensor2_isExpression(tensor_att, i, j)) {
        SimString expression = AttInfoTensor2_expression(tensor_att, i, j);
        values[i][j] = std::stod(std::string(expression));
      } else {
        values[i][j] = AttInfoTensor2_value(tensor_att, i, j);
      }
    }
  }
  return MatrixMT(values);
}
template <> ScalarMT convert<SIMMETRIX>::encode(pAttInfo att, SIMMETRIX *) {
  auto rep_type = AttNode_repType(static_cast<pANode>(att));
  if (rep_type == Att_double) {
    if (AttInfoDouble_isExpression(static_cast<pAttInfoDouble>(att))) {
      SimString expression =
          AttInfoDouble_expression(static_cast<pAttInfoDouble>(att));
      return ScalarMT(std::stod(std::string(expression)));
    } else {
      return ScalarMT(AttInfoDouble_value(static_cast<pAttInfoDouble>(att)));
    }
  }
  if (rep_type == Att_tensor0) {
    if (AttInfoTensor0_isExpression(static_cast<pAttInfoTensor0>(att))) {
      SimString expression =
          AttInfoTensor0_expression(static_cast<pAttInfoTensor0>(att));
      return ScalarMT(std::stod(std::string(expression)));
    } else {
      return ScalarMT(AttInfoTensor0_value(static_cast<pAttInfoTensor0>(att)));
    }
  }
  throw std::logic_error(
      fmt::format("The reptype {} is not convertible to a scalar", rep_type));
}
template <> StringMT convert<SIMMETRIX>::encode(pAttInfo att, SIMMETRIX *) {
  if (AttInfoString_isExpression(static_cast<pAttInfoString>(att))) {
    SimString s = AttInfoString_expression(static_cast<pAttInfoString>(att));
    return StringMT(std::string(s));
  } else {
    SimString s = AttInfoString_value(static_cast<pAttInfoString>(att));
    return StringMT(std::string(s));
  }
}

template <> VectorMT convert<SIMMETRIX>::encode(pAttInfo att, SIMMETRIX *) {
  auto tensor_att = static_cast<pAttInfoTensor1>(att);
  int size = AttInfoTensor1_dimension(tensor_att);
  std::vector<double> values;
  values.reserve(size);
  for (int i = 0; i < size; ++i) {
    if (AttInfoTensor1_isExpression(tensor_att, i)) {
      SimString expression = AttInfoTensor1_expression(tensor_att, i);
      values.push_back(std::stod(std::string(expression)));
    } else {
      values.push_back(AttInfoTensor1_value(tensor_att, i));
    }
  }
  return VectorMT(std::move(values));
}

} // namespace mt