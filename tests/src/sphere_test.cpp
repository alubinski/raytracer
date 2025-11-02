#include "transformations.h"
#include "tuple.h"
#include <catch2/catch.hpp>
#include <cmath>
#include <sphere.h>

TEST_CASE("sphere - normalsAt") {
  SECTION("x axis") {
    const auto s = Sphere();
    const auto n = s.normalsAt(Point(1, 0, 0));
    REQUIRE(n == Vector(1, 0, 0));
  }

  SECTION("y axis") {
    const auto s = Sphere();
    const auto n = s.normalsAt(Point(0, 1, 0));
    REQUIRE(n == Vector(0, 1, 0));
  }

  SECTION("z axis") {
    const auto s = Sphere();
    const auto n = s.normalsAt(Point(0, 0, 1));
    REQUIRE(n == Vector(0, 0, 1));
  }

  SECTION("nonaxial point") {
    const auto s = Sphere();
    const auto n = s.normalsAt(
        Point(std::sqrt(3) / 3, std::sqrt(3) / 3, std::sqrt(3) / 3));
    REQUIRE(n == Vector(std::sqrt(3) / 3, std::sqrt(3) / 3, std::sqrt(3) / 3));
  }

  SECTION("normal is normalized vector") {
    const auto s = Sphere();
    const auto n = s.normalsAt(
        Point(std::sqrt(3) / 3, std::sqrt(3) / 3, std::sqrt(3) / 3));
    REQUIRE(n == n.normalize());
  }

  SECTION("translated sphere") {
    auto s = Sphere();
    s.setTransformation(translation(0, 1, 0));
    const auto n = s.normalsAt(Point(0, 1.70711f, -0.70711f));
    REQUIRE(n == Vector(0, 0.70711f, -0.70711f));
  }

  SECTION("transforrmed sphere") {
    auto s = Sphere();
    s.setTransformation(scaling(1, 0.5, 1) * rotationZ(M_PI / 5));
    const auto n = s.normalsAt(Point(0, std::sqrt(2) / 2, -std::sqrt(2) / 2));
    REQUIRE(n == Vector(0, 0.97014f, -0.24254f));
  }
}
