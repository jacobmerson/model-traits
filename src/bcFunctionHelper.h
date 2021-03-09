#ifndef BC_FUNCTION_HELPER_H__
#define BC_FUNCTION_HELPER_H__

namespace bc {
namespace detail {
template <typename, typename, int, bool, typename... Args> struct FuncCreator;

template <typename R, typename T> struct FuncCreator<R, T, 0, false> {
  using type = R();
};

template <typename R, typename T, int N, typename... Args>
struct FuncCreator<R, T, N, false, Args...> {
  static_assert(sizeof...(Args) < N,
                "This specialization should only occur if the number of "
                "arguments is less than the requested number of arguments.");

  // add one function call type to the type
  using type =
      typename FuncCreator<R, T, N, sizeof...(Args) == N - 1, T, Args...>::type;
};

template <typename R, typename T, int N, typename... Args>
struct FuncCreator<R, T, N, true, Args...> {
  static_assert(sizeof...(Args) == N,
                "This specialization should only occur if the number of "
                "arguments is the same as the requested number of arguments.");
  using type = R(Args...);
};
} // namespace detail
/*
 * Uses recursive template construction to create the type for a
 * function with the return type R and N arguments of type T
 */
template <typename R, typename T, int N>
using FunctionT = typename detail::FuncCreator<R, T, N, false>::type;
} // namespace bc

#endif
