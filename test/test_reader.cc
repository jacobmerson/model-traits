#include <backends/bcBackendYamlCpp.h>
#include <bcModelTraits.h>
#include <fmt/format.h>
#include <fstream>
#include <iostream>

int main(int, char **) {
  std::ifstream istream(
      "/home/mersoj/code/boundary-conditions/test/syntax-test.yaml");
  auto model_traits = bc::ReadFromStream<bc::YAML>(istream);
  // fmt::print("{}\n", *model_traits);
  bc::WriteToStream<bc::YAML>(model_traits.get(), std::cout);
  return 0;
}
