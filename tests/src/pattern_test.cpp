#include "color.h"
#include "pattern.h"
#include "sphere.h"
#include "transformations.h"
#include "tuple.h"
#include <catch2/catch.hpp>
#include <memory>

TEST_CASE("stripe pattern - constructor") {
  const auto pattern = StripePattern(Color::white(), Color::black());
  REQUIRE(pattern.colorA() == Color::white());
  REQUIRE(pattern.colorB() == Color::black());
}

TEST_CASE("stripe pattern - colorAt()") {
  SECTION("stripe pattern is constant in y") {
    const auto pattern = StripePattern(Color::white(), Color::black());
    REQUIRE(pattern.colorAt(Point(0, 0, 0)) == Color::white());
    REQUIRE(pattern.colorAt(Point(0, 1, 0)) == Color::white());
    REQUIRE(pattern.colorAt(Point(0, 2, 0)) == Color::white());
  }

  SECTION("stripe pattern is constant in z") {
    const auto pattern = StripePattern(Color::white(), Color::black());
    REQUIRE(pattern.colorAt(Point(0, 0, 0)) == Color::white());
    REQUIRE(pattern.colorAt(Point(0, 0, 1)) == Color::white());
    REQUIRE(pattern.colorAt(Point(0, 0, 2)) == Color::white());
  }

  SECTION("stripe pattern alternates in x") {
    const auto pattern = StripePattern(Color::white(), Color::black());
    REQUIRE(pattern.colorAt(Point(0, 0, 0)) == Color::white());
    REQUIRE(pattern.colorAt(Point(.9f, 0, 0)) == Color::white());
    REQUIRE(pattern.colorAt(Point(1, 0, 0)) == Color::black());
    REQUIRE(pattern.colorAt(Point(-.1f, 0, 0)) == Color::black());
    REQUIRE(pattern.colorAt(Point(-1.f, 0, 0)) == Color::black());
    REQUIRE(pattern.colorAt(Point(-1.1f, 0, 0)) == Color::white());
  }
}

TEST_CASE("pattern - colorAtObject()") {
  SECTION("with object transformation") {
    auto object = std::make_shared<Sphere>();
    object->transformation() = scaling(2, 2, 2);
    const auto pattern = StripePattern(Color::white(), Color::black());
    const auto c = pattern.colorAtObject(object, Point(1.5f, 0.f, 0.f));
    REQUIRE(c == Color::white());
  }

  SECTION("with pattern transformation") {
    const auto object = std::make_shared<Sphere>();
    auto pattern = StripePattern(Color::white(), Color::black());
    pattern.transformation() = scaling(2, 2, 2);
    const auto c = pattern.colorAtObject(object, Point(1.5f, 0.f, 0.f));
    REQUIRE(c == Color::white());
  }

  SECTION("with object and pattern transformation") {
    auto object = std::make_shared<Sphere>();
    object->transformation() = scaling(2, 2, 2);
    auto pattern = StripePattern(Color::white(), Color::black());
    pattern.transformation() = translation(0.5f, 0.f, 0.f);
    const auto c = pattern.colorAtObject(object, Point(2.5f, 0.f, 0.f));
    REQUIRE(c == Color::white());
  }
}
