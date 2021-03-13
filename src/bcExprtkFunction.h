#ifndef BC_EXPRTK_FUNCTION_H__
#define BC_EXPRTK_FUNCTION_H__
#include "bcTypedefs.h"
#include <array>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

/*
 * The exprtk header is very expensive to include, so we put out exprtk
 * methods into the cpp and explicitly instantiate for various numbers
 * of variables. Note that exprtk is internally limited to working
 * with floating point types.
 */
namespace exprtk {
template <typename T> class expression;
}

namespace bc {

// base template
template <int NumVar = 4, typename T = ScalarType, bool = (NumVar >= 0)>
class ExprtkFunction;
// static size whenever NumVar is a positive number
template <int NumVar, typename T> class ExprtkFunction<NumVar, T, true> {
public:
  using expression_t = exprtk::expression<T>;
  /*
   * Sets up the equation from a string and a list of variable names
   */
  ExprtkFunction(std::string eqn,
                 const std::array<std::string, NumVar> &var_names);
  /*
   * Sets the expression from an equation string. The equation string takes any
   * equation with with variable names marked with a $. Currently only single
   * letter variable names are supported. The variables will be added to the
   * expression in alphabetical order. For example if the expression string is
   * 5*z+6*x*y+t, calling the function will use the order t,x,y,z
   * if there are more variables in the equation than NumVar an exception
   * will be thrown
   */
  explicit ExprtkFunction(std::string);
  // copy and move construction requires recreating the expression from string
  ExprtkFunction(const ExprtkFunction &);
  ExprtkFunction &operator=(const ExprtkFunction &);
  ExprtkFunction(ExprtkFunction &&) noexcept;
  ExprtkFunction &operator=(ExprtkFunction &&) noexcept;
  ~ExprtkFunction() noexcept;
  template <typename... Args> T operator()(Args... args) {
    static_assert(sizeof...(args) == NumVar,
                  "operator() must be called with the same number of arguments "
                  "as the function has variables");
    // note we must copy the arguments into vars_ because the expression
    // holds a reference to vars
    vars_ = {std::move(args)...};
    return GetValue();
  }
  constexpr auto GetNumArgs() noexcept { return NumVar; }
  friend std::string to_string(const ExprtkFunction &f) {
    return f.expression_string_;
  }

private:
  T GetValue();
  std::string expression_string_;
  // vector is used so that the values are stored on the stack,
  // and moving can be done without re-parsing the equation
  std::vector<T> vars_;
  std::unique_ptr<expression_t> expression_;
};
// dynamic size
template <int NumVar, typename T> class ExprtkFunction<NumVar, T, false> {
public:
  using expression_t = exprtk::expression<T>;
  /*
   * Sets the expression from an equation string and a vector of variable names
   */
  ExprtkFunction(std::string eqn, const std::vector<std::string> &var_names);
  /*
   * Sets the expression from an equation string. The equation string takes any
   * equation with with variable names marked with a $. Currently only single
   * letter variable names are supported. The variables will be added to the
   * expression in alphabetical order. For example if the expression string is
   * 5*z+6*x*y+t, calling the function will use the order t,x,y,z
   */
  explicit ExprtkFunction(std::string);
  // copy and move construction requires recreating the expression from string
  ExprtkFunction(const ExprtkFunction &);
  ExprtkFunction &operator=(const ExprtkFunction &);
  ExprtkFunction(ExprtkFunction &&) noexcept;
  ExprtkFunction &operator=(ExprtkFunction &&) noexcept;
  ~ExprtkFunction() noexcept;

  /*
   * Operator to call the underlying expression. This call is not type safe for
   * the number of arguments and therefore it will throw an exception  if the
   * number of arguments used is incorrect.
   */
  template <typename... Args> T operator()(Args... args) {
    if (sizeof...(args) != vars_.size()) {
      throw std::runtime_error(
          "Calling an exprtk function with a different number of arguments "
          "than the number of variables is illegal.");
    }
    vars_ = {std::move(args)...};
    return GetValue();
  }
  T GetValue();
  auto GetNumArgs() noexcept { return vars_.size(); }
  friend std::string to_string(const ExprtkFunction &f) {
    return f.expression_string_;
  }

private:
  std::string expression_string_;
  std::vector<T> vars_;
  // we use a const shared ptr for COW behavior of the expression
  // a ptr type is used for the expression so we can move the exprtk headers
  // to the cpp file. This is important because exprtk is very expensive to
  // include
  std::unique_ptr<expression_t> expression_;
};

} // namespace bc
#endif
