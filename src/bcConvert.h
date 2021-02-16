#ifndef BC_CONVERT_H__
#define BC_CONVERT_H__
#include <type_traits>
namespace bc {

// specialize this struct for your backend and types.
// for each valid conversion set the type alias to
// the backend type for a given BC type. For example
// see bcBackendYamlCppConvert.h
template <typename T, typename Backend> struct BackendTypeMap {};

template <typename T, typename T2, typename Backend = void> struct convert {};

// helper template to return a void type no matter the number of template args
template <typename...> using VoidT = void;
template <typename T, typename Backend, typename = void>
struct BackendHasType : std::false_type {};
template <typename T, typename Backend>
struct BackendHasType<T, Backend,
                      VoidT<typename BackendTypeMap<T, Backend>::type>>
    : std::true_type {};

// an abstract backend struct that all backends derive from
// this is used to enable conversion to/from backends as
// well as
struct Backend {};

template <typename BaseClass> struct Convertible {
  // to function which allows us to pass in the type by reference
  template <typename T,
            typename = std::enable_if_t<!BackendHasType<BaseClass, T>::value>>
  void to(T &val) const {
    static_assert(
        !std::is_base_of<Backend, T>::value,
        "instantiation type for BaseClass is missing from the BackendTypeMap");
    convert<BaseClass, T>::decode(static_cast<const BaseClass &>(*this), val);
  }
  template <typename T,
            typename = std::enable_if_t<BackendHasType<BaseClass, T>::value>>
  void to(typename BackendTypeMap<BaseClass, T>::type &val) const {
    static_assert(std::is_base_of<Backend, T>::value,
                  "Backend (T) not inherited from Backend");
    convert<BaseClass, typename BackendTypeMap<BaseClass, T>::type, T>::decode(
        static_cast<const BaseClass &>(*this), val);
  }
  template <typename T,
            typename = std::enable_if_t<!BackendHasType<BaseClass, T>::value>>
  T to() const {
    static_assert(
        !std::is_base_of<Backend, T>::value,
        "instantiation type for BaseClass is missing from the BackendTypeMap");
    T val;
    convert<BaseClass, T>::decode(static_cast<const BaseClass &>(*this), val);
    return val;
  }
  template <typename T,
            typename = std::enable_if_t<BackendHasType<BaseClass, T>::value>>
  typename BackendTypeMap<BaseClass, T>::type to() const {
    static_assert(std::is_base_of<Backend, T>::value,
                  "Backend (T) not inherited from Backend");
    typename BackendTypeMap<BaseClass, T>::type val;
    convert<BaseClass, typename BackendTypeMap<BaseClass, T>::type, T>::decode(
        static_cast<const BaseClass &>(*this), val);
    return val;
  }

  // from functions
  template <typename T,
            typename = std::enable_if_t<!BackendHasType<BaseClass, T>::value>>
  static BaseClass from(const T &other) {
    static_assert(
        !std::is_base_of<Backend, T>::value,
        "instantiation type for BaseClass is missing from the BackendTypeMap");
    return convert<BaseClass, T>::encode(other);
  }

  template <typename T,
            typename = std::enable_if_t<BackendHasType<BaseClass, T>::value>>
  static BaseClass
  from(const typename BackendTypeMap<BaseClass, T>::type &other) {
    static_assert(std::is_base_of<Backend, T>::value,
                  "Backend (T) not inherited from Backend");
    return convert<BaseClass, typename BackendTypeMap<BaseClass, T>::type,
                   T>::encode(other);
  }
};

} // namespace bc

// adding new types to the readers/writers just requires writing a
// convert struct with the appropriate types.
// for examples look at the various backends

#endif
