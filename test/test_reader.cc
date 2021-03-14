#ifdef BC_HAVE_SIMMETRIX
#include <backends/bcBackendSimmetrix.h>
#endif
#include <backends/bcBackendYamlCpp.h>
#include <iostream>

int main(int, char **) {
  std::ifstream istream("../../test/syntax-test.yaml");
  auto model_traits = mt::ReadFromStream<mt::YAML>(istream);
  mt::WriteToStream<mt::YAML>(model_traits.get(), std::cout);

#ifdef BC_HAVE_SIMMETRIX
  auto model_traits_sim = mt::ReadFromFile<mt::SIMMETRIX>(
      "/lore/mersoj/boundary-conditions/test/test.smd");
  mt::WriteToStream<mt::YAML>(model_traits_sim.get(), std::cout);
#endif
  return 0;
}
