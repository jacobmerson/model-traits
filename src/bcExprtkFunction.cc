#include "bcExprtkFunction.h"
#include "exprtk.hpp"
#include <exception>
#include <fmt/format.h>
namespace bc {

/*
 * Removes the char to_remove from the string
 */
static auto RemoveChar(std::string str, char to_remove) {
  str.erase(remove(begin(str), end(str), to_remove), end(str));
  return str;
}
/*
 * Returns the list of variables in the expression in alphabetical order
 */
static auto GetExpressionVariables(const std::string &expression) {
  using std::find;
  using std::sort;
  auto it_end = end(expression);
  auto it = find(begin(expression), it_end, '$');
  std::vector<std::string> vars;
  while (it != it_end) {
    // add a string of length 1 from the char after the $
    vars.emplace_back(1, *(++it));
    it = find(it, it_end, '$');
  }
  // put the variables into alphabetical order
  sort(begin(vars), end(vars));
  return vars;
}

template <typename T, typename VarNamesT, typename VarsT>
static std::unique_ptr<exprtk::expression<T>>
SetupExprtkExpression(const std::string &expression_string,
                      const VarNamesT &var_names, VarsT &vars) {

  using symbol_table_t = exprtk::symbol_table<T>;
  using parser_t = exprtk::parser<T>;
  symbol_table_t symbol_table;
  if (var_names.size() != vars.size()) {
    throw std::runtime_error(
        "Must have the same number of variables and variable names");
  }
  for (std::size_t i = 0; i < var_names.size(); ++i) {
    symbol_table.add_variable(var_names[i], vars[i]);
  }
  symbol_table.add_constants();
  auto exp = std::make_unique<exprtk::expression<T>>();
  exp->register_symbol_table(symbol_table);

  parser_t parser;
  if (!parser.compile(RemoveChar(expression_string, '$'), *exp)) {
    throw std::runtime_error(fmt::format("Error: {} Expression: {}",
                                         parser.error(), expression_string));
  }
  return exp;
}

template <int NumVar, typename T>
ExprtkFunction<NumVar, T, true>::ExprtkFunction(
    std::string eqn, const std::array<std::string, NumVar> &var_names)
    : expression_string_(std::move(eqn)),
      expression_(
          SetupExprtkExpression<T>(expression_string_, var_names, vars_)) {}

template <int NumVar, typename T>
ExprtkFunction<NumVar, T, true>::ExprtkFunction(std::string eqn)
    : expression_string_(std::move(eqn)) {
  auto v = GetExpressionVariables(expression_string_);
  if (v.size() > NumVar) {
    throw std::runtime_error(fmt::format("{} has more than {} variables.",
                                         expression_string_, NumVar));
  }
  std::array<std::string, NumVar> variable_names;
  std::copy_n(begin(v), v.size(), begin(variable_names));
  expression_ =
      SetupExprtkExpression<T>(expression_string_, variable_names, vars_);
}

template <int NumVar, typename T>
ExprtkFunction<NumVar, T, false>::ExprtkFunction(
    std::string eqn, const std::vector<std::string> &var_names)
    : expression_string_(std::move(eqn)),
      expression_(
          SetupExprtkExpression<T>(expression_string_, var_names, vars_)) {}

template <int NumVar, typename T>
ExprtkFunction<NumVar, T, false>::ExprtkFunction(std::string eqn)
    : expression_string_(std::move(eqn)),
      expression_(SetupExprtkExpression<T>(
          expression_string_, GetExpressionVariables(expression_string_),
          vars_)) {}

template <int NumVar, typename T>
T ExprtkFunction<NumVar, T, true>::GetValue() {
  return expression_->value();
}
template <int NumVar, typename T>
T ExprtkFunction<NumVar, T, false>::GetValue() {
  return expression_->value();
}

// static functions up to size 4
template class ExprtkFunction<1, ScalarType>;
template class ExprtkFunction<2, ScalarType>;
template class ExprtkFunction<3, ScalarType>;
template class ExprtkFunction<4, ScalarType>;
// dynamic sized functions
template class ExprtkFunction<-1, ScalarType>;
} // namespace bc
