#ifndef MODEL_TRAITS_TYPE_TRAITS_H_
#define MODEL_TRAITS_TYPE_TRAITS_H_
#include <type_traits>

namespace mt {
template <typename...> using VoidT = void;
// check if the type has a typedef named type
template <typename, typename = VoidT<>> struct HasType : std::false_type {};
template <typename T>
struct HasType<T, VoidT<typename T::type>> : std::true_type {};
} // namespace mt

#endif
