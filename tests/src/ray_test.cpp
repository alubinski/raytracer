#include "matrix.h"
#include "ray.h"
#include "sphere.h"
#include "transformations.h"
#include "tuple.h"
#include <catch2/catch.hpp>

TEST_CASE("ray - constructor") {
  const auto origin = Point(1, 2, 3);
  const auto direction = Vector(4, 5, 6);
  const auto r = Ray(origin, direction);
  REQUIRE(r.origin() == origin);
  REQUIRE(r.direction() == direction);
}

TEST_CASE("ray - position") {
  const auto r = Ray(Point(2, 3, 4), Vector(1, 0, 0));
  REQUIRE(r.position(0) == Point(2, 3, 4));
  REQUIRE(r.position(1) == Point(3, 3, 4));
  REQUIRE(r.position(-1) == Point(1, 3, 4));
  REQUIRE(r.position(2.5) == Point(4.5, 3, 4));
}

TEST_CASE("ray - intersept") {
  SECTION("sphere - 2 points") {
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    const auto s = std::make_shared<Sphere>();
    const auto xs = r.intersept(s);
    REQUIRE(xs);
    auto &[xs0, xs1] = *xs;
    CHECK(xs0.t() == Approx(4.0));
    CHECK(xs1.t() == Approx(6.0));
  }

  SECTION("sphere - 1 point") {
    const auto r = Ray(Point(0, 1, -5), Vector(0, 0, 1));
    const auto s = std::make_shared<Sphere>();
    const auto xs = r.intersept(s);
    REQUIRE(xs);
    auto &[xs0, xs1] = *xs;
    CHECK(xs0.t() == Approx(5.0));
    CHECK(xs1.t() == Approx(5.0));
  }

  SECTION("sphere - 0 points") {
    const auto r = Ray(Point(0, 2, -5), Vector(0, 0, 1));
    const auto s = std::make_shared<Sphere>();
    const auto xs = r.intersept(s);
    REQUIRE_FALSE(xs);
  }

  SECTION("ray origininates inside spher") {
    const auto r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
    const auto s = std::make_shared<Sphere>();
    const auto xs = r.intersept(s);
    REQUIRE(xs);
    auto &[xs0, xs1] = *xs;
    REQUIRE(xs0.t() == Approx(-1.0));
    REQUIRE(xs1.t() == Approx(1.0));
  }

  SECTION("sphere is behind ray") {
    const auto r = Ray(Point(0, 0, 5), Vector(0, 0, 1));
    const auto s = std::make_shared<Sphere>();
    const auto xs = r.intersept(s);
    REQUIRE(xs);
    auto &[xs0, xs1] = *xs;
    REQUIRE(xs0.t() == Approx(-6.0));
    REQUIRE(xs1.t() == Approx(-4.0));
  }

  SECTION("check object") {
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    const auto s = std::make_shared<Sphere>();
    const auto xs = r.intersept(s);
    REQUIRE(xs);
    auto &[xs0, xs1] = *xs;
    REQUIRE(xs0.object() == s);
    REQUIRE(xs1.object() == s);
  }

  SECTION("with scaled sphere") {
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    const auto s = std::make_shared<Sphere>();
    s->setTransformation(scaling(2, 2, 2));
    const auto xs = r.intersept(s);
    REQUIRE(xs);
    auto &[xs0, xs1] = *xs;
    REQUIRE(xs0.t() == Approx(3));
    REQUIRE(xs1.t() == Approx(7));
  }

  SECTION("with translated sphere") {
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    const auto s = std::make_shared<Sphere>();
    s->setTransformation(translation(5, 0, 0));
    const auto xs = r.intersept(s);
    REQUIRE_FALSE(xs);
  }
}

TEST_CASE("ray - multiplication operator") {
  SECTION("ray * translation_matrix") {
    const auto r = Ray(Point(1, 2, 3), Vector(0, 1, 0));
    const auto m = translation(3, 4, 5);
    const auto r2 = r * m;
    REQUIRE(r2.origin() == Point(4, 6, 8));
    REQUIRE(r2.direction() == Vector(0, 1, 0));
  }

  SECTION("ray * scaling_matrix") {
    const auto r = Ray(Point(1, 2, 3), Vector(0, 1, 0));
    const auto m = scaling(2, 3, 4);
    const auto r2 = r * m;
    REQUIRE(r2.origin() == Point(2, 6, 12));
    REQUIRE(r2.direction() == Vector(0, 3, 0));
  }
}
