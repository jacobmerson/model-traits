#include <iostream>
#include <memory>
#include <model_traits/ModelTraitsIO.h>
int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " input.smd" << std::endl;
    std::exit(1);
  }
  std::string filename{argv[1]};
  auto model_traits = mt::ReadFromFile<mt::SIMMETRIX>(filename);
  auto yaml_backend = std::make_unique<mt::YAML>();
  yaml_backend->default_geometry_type = "dimension id";
  mt::WriteToStream(model_traits.get(), std::cout, yaml_backend.get());
  return 0;
}