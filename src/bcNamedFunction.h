#ifndef BC_NAMED_FUNCTION_H__
#define BC_NAMED_FUNCTION_H__
#include <functional>
#include <string>

namespace bc {
/*
 * trait to check if the type can be converted to a string using
 * the to_string free function.
 */
template <typename T> class IsToStringable {
  template <typename U>
  static std::true_type test(decltype(to_string(std::declval<U>())));
  template <typename U> static std::false_type test(...);

public:
  static constexpr bool value =
      decltype(test<T>(std::declval<std::string>()))::value;
};

// Undefined base NamedFunction class needed for template specialization
template <typename> class NamedFunction;

// type trait to check if a given type is a named function type
template <typename T> struct IsNamedFunction : public std::false_type {};
template <typename T>
struct IsNamedFunction<NamedFunction<T>> : public std::true_type {};
/*
 * NamedFunction is a type erased class that holds a callable function, and
 * provides the to_string utility for that function type. It can be constructed
 * from any callable object that has the to_string function implemented for its
 * type. It also can be constructed from a std::string and a callable object
 * which is convenient for construction from lambda functions. Finally, you can
 * construct with a callable object for the name.
 */
template <typename R, typename... Args> class NamedFunction<R(Args...)> {
public:
  using FunctionT = std::function<R(Args...)>;
  using NameFunctionT = std::function<std::string(const NamedFunction &)>;
  // the number of arguments that
  static constexpr int ArgsCount() { return sizeof...(Args); }

  NamedFunction() noexcept = default;

  // need to have bot the const and non-const overloads of the copy
  // constructor so that they override the forwarding argument constructor
  NamedFunction(NamedFunction &) = default;
  NamedFunction(const NamedFunction &) = default;
  NamedFunction(NamedFunction &&) noexcept = default;
  NamedFunction &operator=(NamedFunction &) = default;
  NamedFunction &operator=(const NamedFunction &) = default;
  NamedFunction &operator=(NamedFunction &&) = default;
  ~NamedFunction() noexcept = default;
  // constructor takes a callable object that has to_string defined for its type
  template <typename Func, typename std::enable_if<
                               std::is_constructible<FunctionT, Func>::value &&
                                   IsToStringable<Func>::value,
                               int>::type = 0>
  NamedFunction(Func &&f)
      : func_(std::forward<Func>(f)), name_([](const NamedFunction &nf) {
          return to_string(
              *(nf.GetFunc().template target<std::decay_t<Func>>()));
        }) {}

  // overload to provide clean error message if incorrect inputs are used for
  // the one argument constructor
  template <typename Func,
            typename std::enable_if<
                !IsToStringable<Func>::value ||
                    !std::is_constructible<FunctionT, Func>::value,
                int>::type = 0>
  NamedFunction(Func) {
    static_assert(IsToStringable<Func>::value,
                  "Functor type must have to_string overload");
    static_assert(
        std::is_constructible<FunctionT, Func>::value,
        "Parameter must be constructible as std::function<R(Args...)>");
  }

  // constructor takes a callable for the name and a callable for the function
  template <typename NameFunc, typename Func,
            typename std::enable_if<
                std::is_constructible<FunctionT, Func>::value &&
                    std::is_constructible<NameFunctionT, NameFunc>::value,
                int>::type = 0>
  NamedFunction(NameFunc &&name, Func &&func)
      : func_(std::forward<Func>(func)), name_(std::forward<NameFunc>(name)) {}
  // overload for clean error message for two functor parameters
  template <typename NameFunc, typename Func,
            typename std::enable_if<
                (!std::is_constructible<FunctionT, Func>::value ||
                 !std::is_constructible<NameFunctionT, NameFunc>::value) &&
                    !std::is_convertible<NameFunc, std::string>::value,
                int>::type = 0>
  NamedFunction(NameFunc, Func) {
    static_assert(
        std::is_constructible<FunctionT, Func>::value,
        "Second parameter must be constructible as std::function<R(Args...)>");
    static_assert(std::is_constructible<NameFunctionT, NameFunc>::value,
                  "First parameter must be constructible as "
                  "std::function<std::string(const NamedFunc&)>");
  }
  // constructor takes a string and a callable
  template <typename Func,
            typename std::enable_if<
                std::is_constructible<FunctionT, Func>::value, int>::type = 0>
  NamedFunction(std::string name, Func &&func)
      : func_(std::forward<Func>(func)),
        name_(
            [name = std::move(name)](const NamedFunction &) { return name; }) {}
  // overload for clean error messages
  template <typename Func,
            typename std::enable_if<
                !std::is_constructible<FunctionT, Func>::value, int>::type = 0>
  NamedFunction(std::string, Func) {
    static_assert(
        std::is_constructible<FunctionT, Func>::value,
        "Second parameter must be constructible as std::function<R(Args...)>");
  }
  // operator() forwards to the operator() of the underlying function type
  typename FunctionT::result_type operator()(Args &&...args) {
    return this->func_(std::forward<Args>(args)...);
  }
  friend std::string to_string(const NamedFunction &nf) { return nf.name_(nf); }
  const FunctionT &GetFunc() const { return func_; }

private:
  FunctionT func_;
  NameFunctionT name_;
};

} // namespace bc

#endif
