#include "color.h"
#include <catch2/catch.hpp>

TEST_CASE("color - Constructor") {
  const auto c = Color(-0.5, 0.4, 2.7);
  REQUIRE(c.r() == Approx(-0.5f));
  REQUIRE(c.g() == Approx(0.4f));
  REQUIRE(c.b() == Approx(2.7f));
}

TEST_CASE("color - Addition operator") {
  const auto c1 = Color(0.9, 0.6, 0.75);
  const auto c2 = Color(0.7, 0.1, 0.25);
  REQUIRE(c1 + c2 == Color(1.6, 0.7, 1.0));
}

TEST_CASE("color - Subtaction operator") {
  const auto c1 = Color(0.9, 0.6, 0.75);
  const auto c2 = Color(0.7, 0.1, 0.25);
  REQUIRE(c1 - c2 == Color(0.2, 0.5, 0.5));
}

TEST_CASE("color - Multiplication operator") {
  SECTION("scalar * color") {
    const auto c = Color(0.2, 0.3, 0.4);
    REQUIRE(c * 2 == Color(0.4, 0.6, 0.8));
  }

  SECTION("color * color") {
    const auto c1 = Color(1, 0.2, 0.4);
    const auto c2 = Color(0.9, 1, 0.1);
    REQUIRE(c1 * c2 == Color(0.9, 0.2, 0.04));
  }
}
