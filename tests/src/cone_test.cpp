
#include "cone.h"
#include "ray.h"
#include "tuple.h"
#include <catch2/catch.hpp>
#include <cmath>
#include <memory>

TEST_CASE("cone - intersept()") {
  auto cone = std::make_shared<Cone>();

  SECTION("standard intersection") {
    auto [origin, direction, t0, t1] = GENERATE(
        std::make_tuple(Point(0, 0, -5), Vector(0, 0, 1), 5.0, 5.0),
        std::make_tuple(Point(0, 0, -5), Vector(1, 1, 1), 8.66025, 8.66025),
        std::make_tuple(Point(1, 1, -5), Vector(-0.5, -1, 1), 4.55006,
                        49.44994));

    Ray r(origin, direction.normalize());
    auto xs = cone->intersept(r);

    REQUIRE(xs.size() == 2);
    CHECK(xs[0].t() == Approx(t0));
    CHECK(xs[1].t() == Approx(t1));
  }

  SECTION("ray paraller to one of cone's halves") {
    Ray r(Point(0, 0, -1), Vector(0, 1, 1).normalize());
    auto xs = cone->intersept(r);

    REQUIRE(xs.size() == 1);
    REQUIRE(xs[0].t() == Approx(0.35355f));
  }

  SECTION("end caps") {
    cone->minimum() = -.5f;
    cone->maximum() = .5f;
    cone->closed() = true;
    auto [origin, direction, count] =
        GENERATE(std::make_tuple(Point(0, 0, -5), Vector(0, 1, 0), 0),
                 std::make_tuple(Point(0, 0, -0.25), Vector(0, 1, 1), 2),
                 std::make_tuple(Point(0, 0, -0.25), Vector(0, 1, 0), 4));
    Ray r(origin, direction.normalize());
    auto xs = cone->intersept(r);

    REQUIRE(xs.size() == count);
  }
}

TEST_CASE("cone - normalsAt()") {
  auto cone = std::make_shared<Cone>();
  auto [point, normal] =
      GENERATE(std::make_tuple(Point(0, 0, 0), Vector(0, 0, 0)),
               std::make_tuple(Point(1, 1, 1), Vector(1, -SQRT_2, 1)),
               std::make_tuple(Point(-1, -1, 0), Vector(-1, 1, 0)));
  const auto n = cone->localNormalsAt(point);
  REQUIRE(n == normal);
}
