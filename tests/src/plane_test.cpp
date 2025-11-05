#include "plane.h"
#include "tuple.h"
#include <catch2/catch.hpp>
#include <memory>

TEST_CASE("plane - localNormalsAt()") {
  const auto p = Plane();
  const auto n1 = p.localNormalsAt(Point(0, 0, 0));
  const auto n2 = p.localNormalsAt(Point(10, 0, -10));
  const auto n3 = p.localNormalsAt(Point(-5, 0, 150));
  REQUIRE(n1 == Vector(0, 1, 0));
  REQUIRE(n2 == Vector(0, 1, 0));
  REQUIRE(n3 == Vector(0, 1, 0));
}

TEST_CASE("plane - intersept") {
  SECTION("ray parallel to the plane") {
    const auto p = Plane();
    const auto r = Ray(Point(0, 10, 0), Vector(0, 0, 1));
    const auto xs = p.intersept(r);
    REQUIRE(xs.empty());
  }

  SECTION("coplanar ray") {
    const auto p = Plane();
    const auto r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
    const auto xs = p.intersept(r);
    REQUIRE(xs.empty());
  }

  SECTION("ray intersecting a plane from above") {
    const auto p = std::make_shared<Plane>();
    const auto r = Ray(Point(0, 1, 0), Vector(0, -1, 0));
    const auto xs = p->intersept(r);
    REQUIRE(xs.size() == 1);
    REQUIRE(xs[0].t() == Approx(1));
    REQUIRE(xs[0].object() == p);
  }

  SECTION("ray intersecting a plane from below") {
    const auto p = std::make_shared<Plane>();
    const auto r = Ray(Point(0, -1, 0), Vector(0, 1, 0));
    const auto xs = p->intersept(r);
    REQUIRE(xs.size() == 1);
    REQUIRE(xs[0].t() == Approx(1));
    REQUIRE(xs[0].object() == p);
  }
}
