#include <iostream>
#include <memory>
#include <model_traits/ModelTraitsIO.h>
int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " input.yaml" << std::endl;
    std::exit(1);
  }
  std::string filename{argv[1]};

  auto yaml_backend = std::make_unique<mt::YAML>();
  auto model_traits = mt::ReadFromFile(filename, yaml_backend.get());

  yaml_backend->model_traits_prefix = "My Application";
  yaml_backend->model_trait_key = "BOUNDARIES";
  yaml_backend->write_geometry_type_per_model_trait = true;
  mt::WriteToStream(model_traits.get(), std::cout, yaml_backend.get());
  return 0;
}