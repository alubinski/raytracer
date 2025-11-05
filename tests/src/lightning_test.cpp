#include "color.h"
#include "lightning.h"
#include "material.h"
#include "tuple.h"
#include <catch2/catch.hpp>
#include <cmath>

TEST_CASE("point_light - constructor") {
  const auto intensity = Color(1, 1, 1);
  const auto position = Point(0, 0, 0);
  const auto light = PointLight(position, intensity);
  REQUIRE(light.position() == position);
  REQUIRE(light.intensity() == intensity);
}

TEST_CASE("lightning") {
  const auto m = Material();
  const auto position = Point(0, 0, 0);

  SECTION("with the eye between the light and the surface") {
    const auto eyeV = Vector(0, 0, -1);
    const auto normalV = Vector(0, 0, -1);
    const auto light = PointLight(Point(0, 0, -10), Color(1, 1, 1));
    const auto result = lightining(m, light, position, eyeV, normalV);
    REQUIRE(result == Color(1.9f, 1.9f, 1.9f));
  }

  SECTION("with the eye between light and surface, eye offset 45°") {
    const auto eyeV = Vector(0, std::sqrt(2) / 2, -std::sqrt(2) / 2);
    const auto normalV = Vector(0, 0, -1);
    const auto light = PointLight(Point(0, 0, -10), Color(1, 1, 1));
    const auto result = lightining(m, light, position, eyeV, normalV);
    REQUIRE(result == Color(1.f, 1.f, 1.f));
  }

  SECTION("with eye opposite surface, light offset 45°") {
    const auto eyeV = Vector(0, 0, -1);
    const auto normalV = Vector(0, 0, -1);
    const auto light = PointLight(Point(0, 10, -10), Color(1, 1, 1));
    const auto result = lightining(m, light, position, eyeV, normalV);
    REQUIRE(result == Color(.7364f, .7364, .7364));
  }

  SECTION("with eye in the path of the reflection vector") {
    const auto eyeV = Vector(0, -std::sqrt(2) / 2, -std::sqrt(2) / 2);
    const auto normalV = Vector(0, 0, -1);
    const auto light = PointLight(Point(0, 10, -10), Color(1, 1, 1));
    const auto result = lightining(m, light, position, eyeV, normalV);
    REQUIRE(result == Color(1.6364f, 1.6364f, 1.6364f));
  }

  SECTION("with the eye between the light and the surface") {
    const auto eyeV = Vector(0, 0, -1);
    const auto normalV = Vector(0, 0, -1);
    const auto light = PointLight(Point(0, 0, 10), Color(1, 1, 1));
    const auto result = lightining(m, light, position, eyeV, normalV);
    REQUIRE(result == Color(.1f, .1f, .1f));
  }

  SECTION("surface in shadow") {
    const auto eyeV = Vector(0, 0, -1);
    const auto normalV = Vector(0, 0, -1);
    const auto light = PointLight(Point(0, 0, -10), Color(1, 1, 1));
    const bool inShadow = true;
    const auto result = lightining(m, light, position, eyeV, normalV, inShadow);
    REQUIRE(result == Color(.1f, .1f, .1f));
  }
}
