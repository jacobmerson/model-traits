#ifdef BC_HAVE_SIMMETRIX
#include <backends/bcBackendSimmetrix.h>
#endif
#include <backends/bcBackendYamlCpp.h>
#include <iostream>

int main(int, char **) {
  std::ifstream istream("../../test/syntax-test.yaml");
  auto model_traits = bc::ReadFromStream<bc::YAML>(istream);
  bc::WriteToStream<bc::YAML>(model_traits.get(), std::cout);

#ifdef BC_HAVE_SIMMETRIX
  auto model_traits_sim = bc::ReadFromFile<bc::SIMMETRIX>(
      "/lore/mersoj/boundary-conditions/test/test.smd");
  bc::WriteToStream<bc::YAML>(model_traits_sim.get(), std::cout);
#endif
  return 0;
}
