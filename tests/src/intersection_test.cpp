#include "computation.h"
#include "intersection.h"
#include "ray.h"
#include "shape.h"
#include "sphere.h"
#include "tuple.h"
#include <catch2/catch.hpp>
#include <cmath>
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

TEST_CASE("schlock()") {
  SECTION("total internal reflection") {
    const auto shape = std::make_shared<Sphere>(Sphere::GlassSphere());
    const auto r = Ray(Point(0, 0, SQRT_2 / 2), Vector(0, 1, 0));
    const auto xs = intersections(Intersection(-SQRT_2 / 2, shape),
                                  Intersection(SQRT_2 / 2, shape));
    const auto comps = r.precompute(xs[1], xs);
    const auto reflectance = schlick(comps);
    REQUIRE(reflectance == Approx(1.f));
  }

  SECTION("total internal reflectiontotal internal reflection") {
    const auto shape = std::make_shared<Sphere>(Sphere::GlassSphere());
    const auto r = Ray(Point(0, 0, 0), Vector(0, 1, 0));
    const auto xs =
        intersections(Intersection(-1, shape), Intersection(1, shape));
    const auto comps = r.precompute(xs[1], xs);
    const auto reflectance = schlick(comps);
    REQUIRE(reflectance == Approx(.04f));
  }

  SECTION("with small angle and n2 > n1") {
    const auto shape = std::make_shared<Sphere>(Sphere::GlassSphere());
    const auto r = Ray(Point(0, 0.99f, -2), Vector(0, 0, 1));
    const auto xs = intersections(Intersection(1.8589, shape));
    const auto comps = r.precompute(xs[0], xs);
    const auto reflectance = schlick(comps);
    REQUIRE(reflectance == Approx(.48873f));
  }
}
