#ifndef BC_TYPE_TRAITS_H__
#define BC_TYPE_TRAITS_H__
#include <type_traits>

namespace bc {
template <typename...> using VoidT = void;
// check if the type has a typedef named type
template <typename, typename = VoidT<>> struct HasType : std::false_type {};
template <typename T>
struct HasType<T, VoidT<typename T::type>> : std::true_type {};
} // namespace bc

#endif
