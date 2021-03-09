#include "bcExprtkFunction.h"
#include <fmt/format.h>
#include <functional>
#include <string>

using ScalarType = double;

auto myfunc(
    std::function<ScalarType(ScalarType, ScalarType, ScalarType, ScalarType)>
        f) {
  return f(1, 1, 1, 2);
}

int main(int, char **argv) {
  bc::ExprtkFunction<4> f("x^2+y", {"x", "y", "z", "t"});
  bc::ExprtkFunction<4> f2("x^2+y", {"x", "y"});
  auto v = f(std::atof(argv[1]), std::atof(argv[2]), std::atof(argv[3]),
             std::atof(argv[4]));
  fmt::print("{}, {}, {}\n", to_string(f), v, myfunc(f));
}
