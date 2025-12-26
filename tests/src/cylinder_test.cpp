#include "cylinder.h"
#include "ray.h"
#include "tuple.h"
#include <catch2/catch.hpp>
#include <cmath>
#include <memory>

TEST_CASE("cylinder - constructor") {
  SECTION("default values") {
    const auto cyl = Cylinder{};

    REQUIRE(cyl.minimum() == -INFINITY);
    REQUIRE(cyl.maximum() == INFINITY);
    REQUIRE_FALSE(cyl.closed());
  }
}

TEST_CASE("cylinder - intersept()") {
  auto cyl = std::make_shared<Cylinder>();

  SECTION("ray misses cylinder") {
    auto [origin, direction] =
        GENERATE(std::make_pair(Point(1, 0, 0), Vector(0, 1, 0)),
                 std::make_pair(Point(0, 0, 0), Vector(0, 1, 0)),
                 std::make_pair(Point(0, 0, -5), Vector(1, 1, 1)));

    Ray r(origin, direction.normalize());
    auto xs = cyl->intersept(r);

    REQUIRE(xs.empty());
  }

  SECTION("ray strike cylinder") {
    auto [origin, direction, t0_expected, t1_expected] =
        GENERATE(std::make_tuple(Point(1, 0, -5), Vector(0, 0, 1), 5.0, 5.0),
                 std::make_tuple(Point(0, 0, -5), Vector(0, 0, 1), 4.0, 6.0),
                 std::make_tuple(Point(0.5, 0, -5), Vector(0.1, 1, 1), 6.80798,
                                 7.08872));

    vector_t dir = direction.normalize();
    Ray r(origin, dir);

    auto xs = cyl->intersept(r);

    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].t() == Approx(t0_expected));
    REQUIRE(xs[1].t() == Approx(t1_expected));
  }

  SECTION("truncated cylinder") {
    cyl->minimum() = 1;
    cyl->maximum() = 2;

    auto [point, direction, expectedCount] =
        GENERATE(std::make_tuple(Point(0, 1.5, 0), Vector(0.1, 1, 0), 0),
                 std::make_tuple(Point(0, 3, -5), Vector(0, 0, 1), 0),
                 std::make_tuple(Point(0, 0, -5), Vector(0, 0, 1), 0),
                 std::make_tuple(Point(0, 2, -5), Vector(0, 0, 1), 0),
                 std::make_tuple(Point(0, 1, -5), Vector(0, 0, 1), 0),
                 std::make_tuple(Point(0, 1.5, -2), Vector(0, 0, 1), 2));

    vector_t dir = direction.normalize();
    Ray r(point, dir);

    auto xs = cyl->intersept(r);

    REQUIRE(xs.size() == expectedCount);
  }

  SECTION("closed cylinder") {
    cyl->minimum() = 1;
    cyl->maximum() = 2;
    cyl->closed() = true;

    auto [point, direction, count] =
        GENERATE(std::make_tuple(Point(0, 3, 0), Vector(0, -1, 0), 2),
                 std::make_tuple(Point(0, 3, -2), Vector(0, -1, 2), 2),
                 std::make_tuple(Point(0, 4, -2), Vector(0, -1, 1), 2),
                 // corner case
                 std::make_tuple(Point(0, 0, -2), Vector(0, 1, 2), 2),
                 // corner case
                 std::make_tuple(Point(0, -1, -2), Vector(0, 1, 1), 2));

    vector_t dir = direction.normalize();
    Ray r(point, dir);

    auto xs = cyl->intersept(r);
    CAPTURE(point, direction, count, xs);

    REQUIRE(xs.size() == count);
  }
}

TEST_CASE("cylinder - normalsAt()") {
  SECTION("Normal vectors on cylinder") {
    const auto cyl = Cylinder{};

    auto [point, expected] =
        GENERATE(std::make_pair(Point(1, 0, 0), Vector(1, 0, 0)),
                 std::make_pair(Point(0, 5, -1), Vector(0, 0, -1)),
                 std::make_pair(Point(0, -2, 1), Vector(0, 0, 1)),
                 std::make_pair(Point(-1, 1, 0), Vector(-1, 0, 0)));

    vector_t n = cyl.normalsAt(point);

    REQUIRE(n == expected);
  }

  SECTION("Normal vectors at cylinder's end caps") {

    auto cyl = Cylinder{};
    cyl.minimum() = 1;
    cyl.maximum() = 2;
    cyl.closed() = true;

    auto [point, expected] = GENERATE(
        // normal = (0, -1, 0)
        std::make_tuple(Point(0, 1, 0), Vector(0, -1, 0)),
        std::make_tuple(Point(0.5, 1, 0), Vector(0, -1, 0)),
        std::make_tuple(Point(0, 1, 0.5), Vector(0, -1, 0)),

        // normal = (0, 1, 0)
        std::make_tuple(Point(0, 2, 0), Vector(0, 1, 0)),
        std::make_tuple(Point(0.5, 2, 0), Vector(0, 1, 0)),
        std::make_tuple(Point(0, 2, 0.5), Vector(0, 1, 0)));

    vector_t n = cyl.normalsAt(point);

    REQUIRE(n == expected);
  }
}
