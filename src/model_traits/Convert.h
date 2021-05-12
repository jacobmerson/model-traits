#ifndef MODEL_TRAITS_CONVERT_H_
#define MODEL_TRAITS_CONVERT_H_
#include <type_traits>
namespace mt {

// you can optionally specialize the BackendTypeMap for your backend types
// If you specialize this struct, it will use the mapped type as the default
// type for when you call the by value to conversion. i.e. this enables you to
// call: auto n = model_traits.to<YAML>(); rather than: auto n =
// model_traits.to<YAML, ::YAML::Node>();
template <typename T, typename Backend> struct BackendTypeMap {};

template <typename Backend = void> struct convert {};

template <typename BaseClass> struct Convertible {
  // convert from that takes T by reference
  template <typename Backend, typename T>
  void to(T &val, Backend *backend = nullptr) const {
    convert<Backend>::decode(static_cast<const BaseClass &>(*this), val,
                             backend);
  }
  // convert from returns the converted item by value
  template <typename Backend, typename T=typename BackendTypeMap<BaseClass,Backend>::type>
  T to(Backend *backend = nullptr) const {
    T val;
    convert<Backend>::decode(static_cast<const BaseClass &>(*this), val,
                             backend);
    return val;
  }

  // Convert from T to the BaseClass type
  template <typename Backend, typename T>
  static BaseClass from(const T &other, Backend *backend = nullptr) {
    return convert<Backend>::template encode<BaseClass>(other, backend);
  }
};

} // namespace mt

// adding new types to the readers/writers just requires writing a
// convert struct with the appropriate types.
// for examples look at the various backends

#endif
