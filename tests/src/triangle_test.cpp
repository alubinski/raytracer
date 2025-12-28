#include "ray.h"
#include "triangle.h"
#include "tuple.h"
#include <catch2/catch.hpp>
#include <memory>

TEST_CASE("triangle - constructor") {
  const auto p1 = Point(0, 1, 0);
  const auto p2 = Point(-1, 0, 0);
  const auto p3 = Point(1, 0, 0);
  const auto t = Triangle(p1, p2, p3);

  REQUIRE(t.p1() == p1);
  REQUIRE(t.p2() == p2);
  REQUIRE(t.p3() == p3);
  REQUIRE(t.e1() == Vector(-1, -1, 0));
  REQUIRE(t.e2() == Vector(1, -1, 0));
  REQUIRE(t.normal() == Vector(0, 0, -1));
}

TEST_CASE("triangle - localNormalAt()") {
  const auto t = Triangle(Point(0, 1, 0), Point(-1, 0, 0), Point(1, 0, 0));
  const auto n1 = t.localNormalsAt(Point(0, 0.5, 0));
  const auto n2 = t.localNormalsAt(Point(-0.5, 0.75, 0));
  const auto n3 = t.localNormalsAt(Point(0.5, 0.25, 0));

  REQUIRE(n1 == t.normal());
  REQUIRE(n2 == t.normal());
  REQUIRE(n3 == t.normal());
}

TEST_CASE("triangle - intersept()") {
  const auto t = std::make_shared<Triangle>(Point(0, 1, 0), Point(-1, 0, 0),
                                            Point(1, 0, 0));

  SECTION("paraller ray") {
    const auto r = Ray(Point(0, -1, -2), Vector(0, 1, 0));
    const auto xs = t->intersept(r);
    REQUIRE(xs.empty());
  }

  SECTION("ray misses the p1-p3 edge") {
    const auto r = Ray(Point(1, 1, -2), Vector(0, 0, 1));
    const auto xs = t->intersept(r);
    REQUIRE(xs.empty());
  }

  SECTION("ray misses the p1-p2 edge") {
    const auto r = Ray(Point(-1, 1, -2), Vector(0, 0, 1));
    const auto xs = t->intersept(r);
    REQUIRE(xs.empty());
  }

  SECTION("ray misses the p2-p3 edge") {
    const auto r = Ray(Point(0, -1, -2), Vector(0, 0, 1));
    const auto xs = t->intersept(r);
    REQUIRE(xs.empty());
  }

  SECTION("ray strike triangle") {
    const auto r = Ray(Point(0, 0.5, -2), Vector(0, 0, 1));
    const auto xs = t->intersept(r);
    REQUIRE(xs.size() == 1);
    REQUIRE(xs[0].t() == Approx(2.f));
  }
}
