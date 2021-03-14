#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"
#pragma ide diagnostic ignored "readability-function-cognitive-complexity"
#include "model_traits/bcExprtkFunction.h"
#include "model_traits/bcNamedFunction.h"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using Catch::Approx;
using mt::ExprtkFunction;
using mt::NamedFunction;

TEST_CASE("static exprtk function", "[exprtk][function]") {
  SECTION("single variable") {
    ExprtkFunction<1> f{"$x"};
    ExprtkFunction<2> f2{"$x"};
    ExprtkFunction<4> f4{"$x"};
    SECTION("initial") {
      REQUIRE(f(1.0) == Approx(1.0));
      REQUIRE(f2(1.0, 2.0) == Approx(1.0));
      REQUIRE(f4(1.0, 2.0, 3.0, 4.0) == Approx(1.0));
      REQUIRE(to_string(f) == "$x");
      REQUIRE(to_string(f2) == "$x");
      REQUIRE(to_string(f4) == "$x");

      REQUIRE(f.GetNumArgs() == 1);
      REQUIRE(f2.GetNumArgs() == 2);
      REQUIRE(f4.GetNumArgs() == 4);
    }
    SECTION("copy constructor") {
      auto fc = f;
      auto f2c = f2;
      auto f4c = f4;
      REQUIRE(f(1.0) == Approx(1.0));
      REQUIRE(f2(1.0, 2.0) == Approx(1.0));
      REQUIRE(f4(1.0, 2.0, 3.0, 4.0) == Approx(1.0));
      REQUIRE(to_string(f) == "$x");
      REQUIRE(to_string(f2) == "$x");
      REQUIRE(to_string(f4) == "$x");
      REQUIRE(fc(1.0) == Approx(1.0));
      REQUIRE(f2c(1.0, 2.0) == Approx(1.0));
      REQUIRE(f4c(1.0, 2.0, 3.0, 4.0) == Approx(1.0));
      REQUIRE(to_string(fc) == "$x");
      REQUIRE(to_string(f2c) == "$x");
      REQUIRE(to_string(f4c) == "$x");
    }
    SECTION("copy assignment") {
      ExprtkFunction<1> fc{"5*$x"};
      ExprtkFunction<2> f2c{"5*$x"};
      ExprtkFunction<4> f4c{"5*$x"};
      fc = f;
      f2c = f2;
      f4c = f4;
      REQUIRE(f(1.0) == Approx(1.0));
      REQUIRE(f2(1.0, 2.0) == Approx(1.0));
      REQUIRE(f4(1.0, 2.0, 3.0, 4.0) == Approx(1.0));
      REQUIRE(to_string(f) == "$x");
      REQUIRE(to_string(f2) == "$x");
      REQUIRE(to_string(f4) == "$x");
      REQUIRE(fc(1.0) == Approx(1.0));
      REQUIRE(f2c(1.0, 2.0) == Approx(1.0));
      REQUIRE(f4c(1.0, 2.0, 3.0, 4.0) == Approx(1.0));
      REQUIRE(to_string(fc) == "$x");
      REQUIRE(to_string(f2c) == "$x");
      REQUIRE(to_string(f4c) == "$x");
    }
    SECTION("move constructor") {
      auto fm = std::move(f);
      auto f2m = std::move(f2);
      auto f4m = std::move(f4);
      REQUIRE(fm(1.0) == Approx(1.0));
      REQUIRE(f2m(1.0, 2.0) == Approx(1.0));
      REQUIRE(f4m(1.0, 2.0, 3.0, 4.0) == Approx(1.0));
      REQUIRE(to_string(fm) == "$x");
      REQUIRE(to_string(f2m) == "$x");
      REQUIRE(to_string(f4m) == "$x");
    }
    SECTION("move assignment") {
      ExprtkFunction<1> fm{"5*$x"};
      ExprtkFunction<2> f2m{"5*$x"};
      ExprtkFunction<4> f4m{"5*$x"};
      fm = std::move(f);
      f2m = std::move(f2);
      f4m = std::move(f4);
      REQUIRE(fm(1.0) == Approx(1.0));
      REQUIRE(f2m(1.0, 2.0) == Approx(1.0));
      REQUIRE(f4m(1.0, 2.0, 3.0, 4.0) == Approx(1.0));
      REQUIRE(to_string(fm) == "$x");
      REQUIRE(to_string(f2m) == "$x");
      REQUIRE(to_string(f4m) == "$x");
    }
  }
  SECTION("two variables") {
    ExprtkFunction<2> f1{"$z/$x"};
    ExprtkFunction<2> f2{"$z/$x", {"z", "x"}};
    ExprtkFunction<2> f3{"$z/$x", {"x", "z"}};
    SECTION("initial") {
      REQUIRE(f1(1.0, 2.0) == Approx(2.0));
      REQUIRE(f2(1.0, 2.0) == Approx(0.5));
      REQUIRE(f3(1.0, 2.0) == Approx(2.0));
      REQUIRE(f1(1.0, 2.0) == Approx(f3(1.0, 2.0)));
      REQUIRE(f1(1.0, 2.0) == Approx(1 / f2(1.0, 2.0)));
      REQUIRE(to_string(f1) == "$z/$x");
      REQUIRE(to_string(f2) == "$z/$x");
      REQUIRE(to_string(f3) == "$z/$x");
    }
    SECTION("copy constructor") {
      auto f1c = f1;
      auto f2c = f2;
      auto f3c = f3;
      REQUIRE(f1c(1.0, 2.0) == Approx(2.0));
      REQUIRE(f2c(1.0, 2.0) == Approx(0.5));
      REQUIRE(f3c(1.0, 2.0) == Approx(2.0));

      REQUIRE(f1c(1.0, 2.0) == Approx(f3(1.0, 2.0)));
      REQUIRE(f1c(1.0, 2.0) == Approx(1 / f2(1.0, 2.0)));
    }
    SECTION("move constructor") {
      auto f1m = std::move(f1);
      auto f2m = std::move(f2);
      auto f3m = std::move(f3);
      REQUIRE(f1m(1.0, 2.0) == Approx(2.0));
      REQUIRE(f2m(1.0, 2.0) == Approx(0.5));
      REQUIRE(f3m(1.0, 2.0) == Approx(2.0));
    }
    SECTION("copy assignment") {
      ExprtkFunction<2> f1c{"100*$z*$x"};
      ExprtkFunction<2> f2c{"100*$z*$x", {"z", "x"}};
      ExprtkFunction<2> f3c{"100*$z*$x", {"x", "z"}};
      f1c = f1;
      f2c = f2;
      f3c = f3;
      REQUIRE(f1c(1.0, 2.0) == Approx(2.0));
      REQUIRE(f2c(1.0, 2.0) == Approx(0.5));
      REQUIRE(f3c(1.0, 2.0) == Approx(2.0));

      REQUIRE(f1c(1.0, 2.0) == Approx(f3(1.0, 2.0)));
      REQUIRE(f1c(1.0, 2.0) == Approx(1 / f2(1.0, 2.0)));
    }
    SECTION("move assignment") {
      ExprtkFunction<2> f1m{"100*$x*$y"};
      ExprtkFunction<2> f2m{"100*$z*$x", {"z", "x"}};
      ExprtkFunction<2> f3m{"100*$z*$x", {"x", "z"}};
      f1m = std::move(f1);
      f2m = std::move(f2);
      f3m = std::move(f3);
      REQUIRE(f1m(1.0, 2.0) == Approx(2.0));
      REQUIRE(f2m(1.0, 2.0) == Approx(0.5));
      REQUIRE(f3m(1.0, 2.0) == Approx(2.0));
    }
  }
}
TEST_CASE("dynamic exprtk function", "[exprtk][function]") {
  ExprtkFunction<-1> f{"$x"};
  ExprtkFunction<-1> f2{"$z/$x"};
  ExprtkFunction<-1> f3{"$z/$x", {"z", "x"}};
  SECTION("initial") {
    // expression with undefined symbols should throw
    REQUIRE_THROWS(ExprtkFunction<-1>{"$z/$x", {"x", "y"}});
    REQUIRE_THROWS(ExprtkFunction<-1>{"$z/$x", {"x", "y", "m"}});

    // check that the function evaluates correctly
    REQUIRE(f(1.0) == Approx(1.0));
    REQUIRE(f2(1.0, 2.0) == Approx(2.0));
    REQUIRE(f3(1.0, 2.0) == Approx(0.5));
    REQUIRE(f.GetNumArgs() == 1);
    REQUIRE(f2.GetNumArgs() == 2);
    REQUIRE(f3.GetNumArgs() == 2);

    // calling the function with the wrong number of arguments
    // is an exception
    REQUIRE_THROWS(f(1.0, 2.0));
    REQUIRE_THROWS(f2(1.0));
    REQUIRE_THROWS(f2(1.0, 2.0, 3.0));
    REQUIRE_THROWS(f3(1.0));
    REQUIRE_THROWS(f3(1.0, 2.0, 3.0));
  }
  SECTION("copy constructor") {
    auto fc = f;
    auto f2c = f2;
    auto f3c = f3;
    REQUIRE(fc(1.0) == Approx(1.0));
    REQUIRE(f2c(1.0, 2.0) == Approx(2.0));
    REQUIRE(f3c(1.0, 2.0) == Approx(0.5));
    REQUIRE(fc.GetNumArgs() == 1);
    REQUIRE(f2c.GetNumArgs() == 2);
    REQUIRE(f3c.GetNumArgs() == 2);
  }
  SECTION("move constructor") {
    auto fm = std::move(f);
    auto f2m = std::move(f2);
    auto f3m = std::move(f3);
    REQUIRE(fm(1.0) == Approx(1.0));
    REQUIRE(f2m(1.0, 2.0) == Approx(2.0));
    REQUIRE(f3m(1.0, 2.0) == Approx(0.5));
    REQUIRE(fm.GetNumArgs() == 1);
    REQUIRE(f2m.GetNumArgs() == 2);
    REQUIRE(f3m.GetNumArgs() == 2);
  }
  SECTION("copy assignment") {
    ExprtkFunction<-1> fc{"100*$x*$y*$z"};
    ExprtkFunction<-1> f2c{"100*$x*$y*$z"};
    ExprtkFunction<-1> f3c{"100*$x*$y*$z", {"x", "y", "z"}};
    fc = f;
    f2c = f2;
    f3c = f3;
    REQUIRE(fc(1.0) == Approx(1.0));
    REQUIRE(f2c(1.0, 2.0) == Approx(2.0));
    REQUIRE(f3c(1.0, 2.0) == Approx(0.5));
    REQUIRE(fc.GetNumArgs() == 1);
    REQUIRE(f2c.GetNumArgs() == 2);
    REQUIRE(f3c.GetNumArgs() == 2);
  }
  SECTION("move assignment") {
    ExprtkFunction<-1> fm{"100*$x*$y*$z"};
    ExprtkFunction<-1> f2m{"100*$x*$y*$z"};
    ExprtkFunction<-1> f3m{"100*$x*$y*$z", {"x", "y", "z"}};
    fm = std::move(f);
    f2m = std::move(f2);
    f3m = std::move(f3);
    REQUIRE(fm(1.0) == Approx(1.0));
    REQUIRE(f2m(1.0, 2.0) == Approx(2.0));
    REQUIRE(f3m(1.0, 2.0) == Approx(0.5));
    REQUIRE(fm.GetNumArgs() == 1);
    REQUIRE(f2m.GetNumArgs() == 2);
    REQUIRE(f3m.GetNumArgs() == 2);
  }
}

struct TestFunctor {
  double operator()(double a, double b) { return a / b; }
  friend std::string to_string(const TestFunctor &) { return "Named Functor"; }
};

TEST_CASE("NamedFunction", "[function]") {
  NamedFunction<int(double)> lambda{"Named Lambda", [](double a) { return a; }};
  NamedFunction<double(double, double)> functor{TestFunctor{}};
  NamedFunction<double(double, double)> functor2{"Functor 2", TestFunctor{}};
  NamedFunction<double(double, double, double)> exprtk{
      ExprtkFunction<3>{"$x/$y+$z"}};
  SECTION("initial") {
    // lambda
    REQUIRE(lambda(8.0) == Approx(8));
    REQUIRE(to_string(lambda) == "Named Lambda");
    // functor
    REQUIRE(functor(1.0, 2.0) == Approx(0.5));
    REQUIRE(to_string(functor) == "Named Functor");
    REQUIRE(functor2(1.0, 2.0) == Approx(0.5));
    // functor with explicit name uses it
    REQUIRE(to_string(functor2) == "Functor 2");
    // exprtk
    REQUIRE(exprtk(1.0, 2.0, 5.0) == Approx(5.5));
    REQUIRE(to_string(exprtk) == "$x/$y+$z");
  }
  SECTION("copy constructor") {
    auto lambda_copy = lambda;
    auto functor_copy = functor;
    auto functor2_copy = functor2;
    auto exprtk_copy = exprtk;
    REQUIRE(lambda_copy(8.0) == Approx(8));
    REQUIRE(to_string(lambda_copy) == "Named Lambda");
    // // functor
    REQUIRE(functor_copy(1.0, 2.0) == Approx(0.5));
    REQUIRE(to_string(functor_copy) == "Named Functor");
    REQUIRE(functor2_copy(1.0, 2.0) == Approx(0.5));
    // // functor with explicit name uses it
    REQUIRE(to_string(functor2_copy) == "Functor 2");
    // // exprtk
    REQUIRE(exprtk_copy(1.0, 2.0, 5.0) == Approx(5.5));
    REQUIRE(to_string(exprtk_copy) == "$x/$y+$z");
  } //
  SECTION("copy assignment") {
    NamedFunction<int(double)> lambda_copy;
    NamedFunction<double(double, double)> functor_copy;
    NamedFunction<double(double, double)> functor2_copy;
    NamedFunction<double(double, double, double)> exprtk_copy;
    lambda_copy = lambda;
    functor_copy = functor;
    functor2_copy = functor2;
    exprtk_copy = exprtk;
    REQUIRE(lambda_copy(8.0) == Approx(8));
    REQUIRE(to_string(lambda_copy) == "Named Lambda");
    // functor
    REQUIRE(functor_copy(1.0, 2.0) == Approx(0.5));
    REQUIRE(to_string(functor_copy) == "Named Functor");
    REQUIRE(functor2_copy(1.0, 2.0) == Approx(0.5));
    // functor with explicit name uses it
    REQUIRE(to_string(functor2_copy) == "Functor 2");
    // exprtk
    REQUIRE(exprtk_copy(1.0, 2.0, 5.0) == Approx(5.5));
    REQUIRE(to_string(exprtk_copy) == "$x/$y+$z");
  }
  SECTION("move constructor") {
    auto lambda_move = std::move(lambda);
    auto functor_move = std::move(functor);
    auto functor2_move = std::move(functor2);
    auto exprtk_move = std::move(exprtk);
    REQUIRE(lambda_move(8.0) == Approx(8));
    REQUIRE(to_string(lambda_move) == "Named Lambda");
    // functor
    REQUIRE(functor_move(1.0, 2.0) == Approx(0.5));
    REQUIRE(to_string(functor_move) == "Named Functor");
    REQUIRE(functor2_move(1.0, 2.0) == Approx(0.5));
    // functor with explicit name uses it
    REQUIRE(to_string(functor2_move) == "Functor 2");
    // exprtk
    REQUIRE(exprtk_move(1.0, 2.0, 5.0) == Approx(5.5));
    REQUIRE(to_string(exprtk_move) == "$x/$y+$z");
  }
  SECTION("move assignment") {
    NamedFunction<int(double)> lambda_move;
    NamedFunction<double(double, double)> functor_move;
    NamedFunction<double(double, double)> functor2_move;
    NamedFunction<double(double, double, double)> exprtk_move;
    lambda_move = std::move(lambda);
    functor_move = std::move(functor);
    functor2_move = std::move(functor2);
    exprtk_move = std::move(exprtk);
    REQUIRE(lambda_move(8.0) == Approx(8));
    REQUIRE(to_string(lambda_move) == "Named Lambda");
    // functor
    REQUIRE(functor_move(1.0, 2.0) == Approx(0.5));
    REQUIRE(to_string(functor_move) == "Named Functor");
    REQUIRE(functor2_move(1.0, 2.0) == Approx(0.5));
    // functor with explicit name uses it
    REQUIRE(to_string(functor2_move) == "Functor 2");
    // exprtk
    REQUIRE(exprtk_move(1.0, 2.0, 5.0) == Approx(5.5));
    REQUIRE(to_string(exprtk_move) == "$x/$y+$z");
  }
}
#pragma clang diagnostic pop