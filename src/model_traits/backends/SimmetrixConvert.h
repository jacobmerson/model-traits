#ifndef BC_BACKEND_SIMMETRIX_CONVERT_H
#define BC_BACKEND_SIMMETRIX_CONVERT_H
#include "Simmetrix.h"
#include "SimmetrixWrapper.h"
#include "model_traits/Convert.h"
#include "model_traits/ExprtkFunction.h"
#include <SimAttribute.h>

namespace mt {
template <> struct convert<SIMMETRIX> {
  // template <typename R, typename T> static R encode(const T &, SIMMETRIX *);

  template <typename R, std::enable_if_t<!IsFunctionMT<R>::value, bool> = false>
  static R encode(pAttInfo, SIMMETRIX *);
  // overloads for scalar, vector and tensor functions
  // SFINAE is used since the method to encode the functions of each type is
  // identical
  template <
      typename R,
      std::enable_if_t<IsFunctionMT<R>::value && (R::dim == 0), bool> = false>
  static R encode(pAttInfo, SIMMETRIX * /*unused*/);
  template <
      typename R,
      std::enable_if_t<IsFunctionMT<R>::value && (R::dim == 1), bool> = false>
  static R encode(pAttInfo, SIMMETRIX * /*unused*/);
  template <
      typename R,
      std::enable_if_t<IsFunctionMT<R>::value && (R::dim == 2), bool> = false>
  static R encode(pAttInfo, SIMMETRIX * /*unused*/);
};
template <typename R,
          std::enable_if_t<IsFunctionMT<R>::value && (R::dim == 0), bool>>
R convert<SIMMETRIX>::encode(pAttInfo att, SIMMETRIX * /*unused*/) {
  auto rep_type = AttNode_repType(static_cast<pANode>(att));
  SimString exp;
  switch (rep_type) {
  case Att_int:
    exp = std::move(AttInfoInt_expression(static_cast<pAttInfoInt>(att)));
    break;
  case Att_double:
    exp = std::move(AttInfoDouble_expression(static_cast<pAttInfoDouble>(att)));
    break;
  case Att_tensor0:
    exp =
        std::move(AttInfoTensor0_expression(static_cast<pAttInfoTensor0>(att)));
    break;
  default:
    throw std::runtime_error(
        "Attempting to add a scalar equation with unhandled attribute type.");
  }

  return R{ExprtkFunction<R::type::ArgsCount()>{std::string(exp)}};
}
template <typename R,
          std::enable_if_t<IsFunctionMT<R>::value && (R::dim == 1), bool>>
R convert<SIMMETRIX>::encode(pAttInfo att, SIMMETRIX * /*unused*/) {
  auto *tensor_att = static_cast<pAttInfoTensor1>(att);
  auto size = AttInfoTensor1_dimension(tensor_att);
  std::vector<typename R::type> functions;
  for (int i = 0; i < size; ++i) {
    std::string eqn;
    if (AttInfoTensor1_isExpression(tensor_att, i)) {
      // first construct the string as a SimString so the
      // memory will be properly cleaned
      SimString s = AttInfoTensor1_expression(tensor_att, i);
      eqn = std::string(s);
    } else {
      std::to_string(AttInfoTensor1_value(tensor_att, i));
    }
    functions.emplace_back(
        ExprtkFunction<R::type::ArgsCount()>{std::move(eqn)});
  }
  return R{std::move(functions)};
}
template <typename R,
          std::enable_if_t<IsFunctionMT<R>::value && (R::dim == 2), bool>>
R convert<SIMMETRIX>::encode(pAttInfo att, SIMMETRIX * /*unused*/) {
  auto *tensor_att = static_cast<pAttInfoTensor2>(att);
  auto size = AttInfoTensor2_dimension(tensor_att);
  std::vector<std::vector<typename R::type>> functions;
  for (int i = 0; i < size; ++i) {
    functions.emplace_back();
    for (int j = 0; j < size; ++j) {
      std::string eqn;
      if (AttInfoTensor2_isExpression(tensor_att, i, j)) {
        // first construct the string as a SimString so the
        // memory will be properly cleaned
        SimString s = AttInfoTensor2_expression(tensor_att, i, j);
        eqn = std::string(s);
      } else {
        std::to_string(AttInfoTensor2_value(tensor_att, i, j));
      }
      functions.back().emplace_back(
          ExprtkFunction<R::type::ArgsCount()>{std::move(eqn)});
    }
  }
  return R{std::move(functions)};
}
} // namespace mt

#endif
