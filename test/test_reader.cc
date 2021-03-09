#include <backends/bcBackendSimmetrix.h>
#include <backends/bcBackendYamlCpp.h>
#include <iostream>

int main(int, char **) {
  std::ifstream istream("../../test/syntax-test.yaml");
  auto model_traits = bc::ReadFromStream<bc::YAML>(istream);

  // auto model_traits =
  auto model_traits_sim = bc::ReadFromFile<bc::SIMMETRIX>(
      "/lore/mersoj/boundary-conditions/test/test.smd");
  bc::WriteToStream<bc::YAML>(model_traits_sim.get(), std::cout);
  // fmt::print("{}\n", *model_traits);
  bc::WriteToStream<bc::YAML>(model_traits.get(), std::cout);
  return 0;
}
