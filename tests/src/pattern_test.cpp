#include "color.h"
#include "pattern.h"
#include "tuple.h"
#include <catch2/catch.hpp>

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
