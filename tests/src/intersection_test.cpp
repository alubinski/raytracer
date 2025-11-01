#include "intersection.h"
#include "sphere.h"
#include <catch2/catch.hpp>
#include <memory>

TEST_CASE("intersection - constructor") {
  const auto s = std::make_shared<Sphere>();
  const auto i = Intersection(3.5, s);
  REQUIRE(i.t() == Approx(3.5));
  REQUIRE(i.object() == s);
}

TEST_CASE("intersection - aggregate") {
  const auto s = std::make_shared<Sphere>();
  const auto i1 = Intersection(1.0, s);
  const auto i2 = Intersection(2.0, s);
  const auto xs = intersections(i1, i2);
  REQUIRE(xs.size() == 2);
  REQUIRE(xs[0].t() == Approx(1.0));
  REQUIRE(xs[1].t() == Approx(2.0));
}

TEST_CASE("intersection - hit") {
  SECTION("2 positive t") {
    const auto s = std::make_shared<Sphere>();
    const auto i1 = Intersection(1.0, s);
    const auto i2 = Intersection(2.0, s);
    const auto xs = intersections(i2, i1);
    const auto i = hit(xs);
    REQUIRE(i == i1);
  }

  SECTION("1 positive t & 1 negative t") {
    const auto s = std::make_shared<Sphere>();
    const auto i1 = Intersection(-1.0, s);
    const auto i2 = Intersection(1.0, s);
    const auto xs = intersections(i2, i1);
    const auto i = hit(xs);
    REQUIRE(i == i2);
  }

  SECTION("2 negative t") {
    const auto s = std::make_shared<Sphere>();
    const auto i1 = Intersection(-2.0, s);
    const auto i2 = Intersection(-1.0, s);
    const auto xs = intersections(i2, i1);
    const auto i = hit(xs);
    REQUIRE_FALSE(i);
  }

  SECTION("multiple values") {
    const auto s = std::make_shared<Sphere>();
    const auto i1 = Intersection(5, s);
    const auto i2 = Intersection(7, s);
    const auto i3 = Intersection(-3, s);
    const auto i4 = Intersection(2, s);
    const auto xs = intersections(i1, i2, i3, i4);
    const auto i = hit(xs);
    REQUIRE(i == i4);
  }
}
