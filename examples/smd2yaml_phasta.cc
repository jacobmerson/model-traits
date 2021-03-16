#include <iostream>
#include <memory>
#include <model_traits/ModelTraitsIO.h>
#include <string>

std::string phasta_type_map(const std::string &name) {
  if (name == "rigid body" || name == "material type" ||
      name == "DG interface" || name == "periodic slave") {
    return "int";
  }
  // since comp has vector magnitude which is a scalar, make sure that
  // vector magnitude is set to scalar type before vector is set to vector type
  if (name.find("scalar") != std::string::npos ||
      name.find("pressure") != std::string::npos ||
      name.find("temperature") != std::string::npos ||
      name.find("magnitude") != std::string::npos || name == "mass flux" ||
      name == "heat flux" || name == "density" || name == "turbulence wall" ||
      name == "surf ID") {
    return "scalar";
  }
  if (name.find("vector") != std::string::npos ||
      name.find("velocity") != std::string::npos) {
    return "vector";
  }

  return "";
}
int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " input.smd" << std::endl;
    std::exit(1);
  }
  std::string filename{argv[1]};
  auto model_traits = mt::ReadFromFile<mt::SIMMETRIX>(filename);
  auto yaml_backend = std::make_unique<mt::YAML>();
  yaml_backend->default_geometry_type = "dimension id";
  yaml_backend->name2type = phasta_type_map;
  mt::WriteToStream(model_traits.get(), std::cout, yaml_backend.get());
  return 0;
}
