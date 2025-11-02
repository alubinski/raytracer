#include "catch2/catch.hpp"
#include "material.h"

TEST_CASE("material - default constructor") {
  const auto m = Material();
  REQUIRE(m.color() == Color(1, 1, 1));
  REQUIRE(m.ambient() == Approx(0.1f));
  REQUIRE(m.diffuse() == Approx(0.9f));
  REQUIRE(m.specular() == Approx(0.9f));
  REQUIRE(m.shiness() == Approx(200.f));
}
