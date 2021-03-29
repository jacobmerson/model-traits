#ifndef BC_BACKEND_YAML_CPP_H__
#define BC_BACKEND_YAML_CPP_H__
#include <functional>
#include <string>

namespace mt {
struct YAML {
  /**
   * A function pointer that maps the name of a model trait to its associated
   * type. For example, if the name is pressure, you may want to return that it
   * is a scalar type. In this case you could use
   * [](const std::string& name){if(name=="pressure") return "scalar"; return
   * "";} For any unhandled cases, the provided function should return an empty
   * string.
   */
  std::function<std::string(const std::string &)> name2type;
  /**
   * The default geometry type that will be used if
   */
  std::string default_geometry_type = "id";
  /**
   * if this is true, the geometry type will be written on a per model trait
   * basis. In most cases, all of the geometry in the model will be of the same
   * type, so setting the default geometry type is sufficient
   */
  bool write_geometry_type_per_model_trait = false;
  /**
   * If this is true, the type will be written even if the name is mapped
   * using the name2type function.
   */
  bool write_trait_type_on_named_types = false;

  /**
   * keyword that is used to denote the portions of the yaml file
   * that should be read by the model_traits software
   */
  std::string model_traits_prefix = "model traits";
};
} // namespace mt

#endif
