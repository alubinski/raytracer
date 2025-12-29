#include "intersection.h"
#include "ray.h"
#include "smooth_triangle.h"
#include "tuple.h"
#include <catch2/catch.hpp>
#include <memory>

TEST_CASE("smooth triangle - constructor") {
  const auto p1 = Point(0, 1, 0);
  const auto p2 = Point(-1, 0, 0);
  const auto p3 = Point(1, 0, 0);

  const auto n1 = Vector(0, 1, 0);
  const auto n2 = Vector(-1, 0, 0);
  const auto n3 = Vector(1, 0, 0);

  const SmoothTriangle tri(p1, p2, p3, n1, n2, n3);
  REQUIRE(tri.p1() == p1);
  REQUIRE(tri.p2() == p2);
  REQUIRE(tri.p3() == p3);
  REQUIRE(tri.n1() == n1);
  REQUIRE(tri.n2() == n2);
  REQUIRE(tri.n3() == n3);
}

TEST_CASE("smooth triangle - intersept()") {
  const auto p1 = Point(0, 1, 0);
  const auto p2 = Point(-1, 0, 0);
  const auto p3 = Point(1, 0, 0);

  const auto n1 = Vector(0, 1, 0);
  const auto n2 = Vector(-1, 0, 0);
  const auto n3 = Vector(1, 0, 0);

  const auto tri = std::make_shared<SmoothTriangle>(p1, p2, p3, n1, n2, n3);
  const Ray r{Point(-0.2, 0.3, -2), Vector(0, 0, 1)};
  const auto xs = tri->intersept(r);
  REQUIRE(xs[0].u() == Approx(.45f));
  REQUIRE(xs[0].v() == Approx(.25f));
}

TEST_CASE("smooth triangle - normalsAt()") {
  const auto p1 = Point(0, 1, 0);
  const auto p2 = Point(-1, 0, 0);
  const auto p3 = Point(1, 0, 0);

  const auto n1 = Vector(0, 1, 0);
  const auto n2 = Vector(-1, 0, 0);
  const auto n3 = Vector(1, 0, 0);

  const auto tri = std::make_shared<SmoothTriangle>(p1, p2, p3, n1, n2, n3);
  const Intersection i(1, tri, .45f, .25f);
  const auto n = tri->normalsAt(Point(0, 0, 0), i);
  REQUIRE(n == Vector(-0.5547, 0.83205, 0));
}

TEST_CASE("smooth triangle - prepare computations") {
  const auto p1 = Point(0, 1, 0);
  const auto p2 = Point(-1, 0, 0);
  const auto p3 = Point(1, 0, 0);

  const auto n1 = Vector(0, 1, 0);
  const auto n2 = Vector(-1, 0, 0);
  const auto n3 = Vector(1, 0, 0);

  const auto tri = std::make_shared<SmoothTriangle>(p1, p2, p3, n1, n2, n3);
  const Intersection i(1, tri, .45f, .25f);
  const Ray r{Point(-0.2, 0.3, -2), Vector(0, 0, 1)};
  const auto xs = intersections(i);
  const auto comps = r.precompute(i, xs);
  REQUIRE(comps.normalV == Vector(-0.5547, 0.83205, 0));
}
