#include "color.h"
#include "pattern.h"
#include "sphere.h"
#include "transformations.h"
#include "tuple.h"
#include <catch2/catch.hpp>
#include <memory>

class TestPattern : public Pattern {
  Color colorAt(const point_t &point) const override {
    return Color(point.x, point.y, point.z);
  }
  bool operator==(PatternPtr const &other) const override {
    return std::dynamic_pointer_cast<TestPattern>(other) != nullptr;
  }
};

TEST_CASE("pattern - transformation()") {
  const auto pattern = TestPattern();
  REQUIRE(pattern.transformation() == Mat44::identity());
}

TEST_CASE("pattern - setTransformation()") {
  auto pattern = TestPattern();
  pattern.setTransformation(translation(1, 2, 3));
  REQUIRE(pattern.transformation() == translation(1, 2, 3));
}

TEST_CASE("pattern - colorAtObject") {
  SECTION("with object transformation") {
    auto shape = std::make_shared<Sphere>();
    shape->transformation() = scaling(2, 2, 2);
    const auto pattern = TestPattern();
    const auto c = pattern.colorAtObject(shape, Point(2, 3, 4));
    REQUIRE(c == Color(1, 1.5, 2));
  }

  SECTION("with pattern transformation") {
    const auto shape = std::make_shared<Sphere>();
    auto pattern = TestPattern();
    pattern.transformation() = scaling(2, 2, 2);
    const auto c = pattern.colorAtObject(shape, Point(2, 3, 4));
    REQUIRE(c == Color(1, 1.5, 2));
  }

  SECTION("with object and pattern transformation") {
    auto shape = std::make_shared<Sphere>();
    shape->transformation() = scaling(2, 2, 2);
    auto pattern = TestPattern();
    pattern.transformation() = translation(.5f, 1.f, 1.5f);
    const auto c = pattern.colorAtObject(shape, Point(2.5f, 3.f, 3.5f));
    REQUIRE(c == Color(.75f, .5f, .25f));
  }
}

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

TEST_CASE("gradient pattern - colorAt()") {
  const auto pattern = GradientPattern(Color::white(), Color::black());
  REQUIRE(pattern.colorAt(Point(0.f, 0.f, 0.f)) == Color::white());
  REQUIRE(pattern.colorAt(Point(.25f, 0.f, 0.f)) == Color(.75, .75f, .75f));
  REQUIRE(pattern.colorAt(Point(.5f, 0.f, 0.f)) == Color(.5, .5f, .5f));
  REQUIRE(pattern.colorAt(Point(.75f, 0.f, 0.f)) == Color(.25, .25f, .25f));
}

TEST_CASE("ring pattern - colorAt()") {
  const auto pattern = RingPattern(Color::white(), Color::black());
  REQUIRE(pattern.colorAt(Point(0.f, 0.f, 0.f)) == Color::white());
  REQUIRE(pattern.colorAt(Point(1.f, 0.f, 0.f)) == Color::black());
  REQUIRE(pattern.colorAt(Point(0.f, 0.f, 1.f)) == Color::black());
  REQUIRE(pattern.colorAt(Point(.708f, 0.f, .708f)) == Color::black());
}

TEST_CASE("checked pattern - colorAt()") {
  SECTION("checkers should repeat in x") {
    const auto pattern = CheckerPattern(Color::white(), Color::black());
    REQUIRE(pattern.colorAt(Point(0.f, 0.f, 0.f)) == Color::white());
    REQUIRE(pattern.colorAt(Point(.99f, 0.f, 0.f)) == Color::white());
    REQUIRE(pattern.colorAt(Point(1.01f, 0.f, 0.f)) == Color::black());
  }

  SECTION("checkers should repeat in y") {
    const auto pattern = CheckerPattern(Color::white(), Color::black());
    REQUIRE(pattern.colorAt(Point(0.f, 0.f, 0.f)) == Color::white());
    REQUIRE(pattern.colorAt(Point(0.f, .99f, 0.f)) == Color::white());
    REQUIRE(pattern.colorAt(Point(0.f, 1.01f, 0.f)) == Color::black());
  }

  SECTION("checkers should repeat in y") {
    const auto pattern = CheckerPattern(Color::white(), Color::black());
    REQUIRE(pattern.colorAt(Point(0.f, 0.f, 0.f)) == Color::white());
    REQUIRE(pattern.colorAt(Point(0.f, 0.f, .99f)) == Color::white());
    REQUIRE(pattern.colorAt(Point(0.f, 0.f, 1.01f)) == Color::black());
  }
}
