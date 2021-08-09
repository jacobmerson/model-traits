#include <iostream>
#include <memory>
#include <model_traits/ModelTraitsIO.h>
int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " input.yaml" << std::endl;
    std::exit(1);
  }
  std::string filename{argv[1]};

  // Create a pointer to a YAML backend instance.
  // This instance will be used to set parameters during the writing phase.
  auto yaml_backend = std::make_unique<mt::YAML>();

  // load the model traits from yaml file
  auto model_traits = mt::ReadFromFile(filename, yaml_backend.get());
  
  // set the model traits prefix type which will be written to output yaml file
  yaml_backend->model_traits_prefix = "My Application";

  // set the backend to write the geometry type in every model type rather than
  // once at the beginning of the case
  yaml_backend->write_geometry_type_per_model_trait = true;

  // write model traits to stdout using the yaml_backend with it's modified settings
  mt::WriteToStream(model_traits.get(), std::cout, yaml_backend.get());

  return 0;
}
