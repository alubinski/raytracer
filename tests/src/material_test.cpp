#include "catch2/catch.hpp"
#include "lightning.h"
#include "material.h"
#include "sphere.h"
#include "tuple.h"
#include <memory>

TEST_CASE("material - default constructor") {
  const auto m = Material();
  REQUIRE(m.color() == Color(1, 1, 1));
  REQUIRE(m.ambient() == Approx(0.1f));
  REQUIRE(m.diffuse() == Approx(0.9f));
  REQUIRE(m.specular() == Approx(0.9f));
  REQUIRE(m.shiness() == Approx(200.f));
  REQUIRE(m.reflective() == Approx(0.f));
  REQUIRE(m.transparency() == Approx(0.f));
  REQUIRE(m.reflectiveIndex() == Approx(1.f));
}

TEST_CASE("lightning with pattern applied") {
  Material m{};
  m.pattern() = std::make_shared<StripePattern>(Color::white(), Color::black());
  m.setAmbient(1.f);
  m.setDiffuse(0.f);
  m.setSpecular(0.f);
  const auto s = std::make_shared<Sphere>();
  const auto eyev = Vector(0, 0, -1);
  const auto normalv = Vector(0, 0, -1);
  const auto light = PointLight(Point(0, 0, -10), Color::white());
  const auto c1 =
      lightining(m, s, light, Point(0.9f, 0.f, 0.f), eyev, normalv, false);
  const auto c2 =
      lightining(m, s, light, Point(1.1f, 0.f, 0.f), eyev, normalv, false);
  REQUIRE(c1 == Color::white());
  REQUIRE(c2 == Color::black());
}
