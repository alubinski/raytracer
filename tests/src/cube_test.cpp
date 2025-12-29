#include "cube.h"
#include "intersection.h"
#include "ray.h"
#include <catch2/catch.hpp>
#include <memory>

TEST_CASE("cube - intersect()") {
  auto [label, origin, direction, t1, t2] = GENERATE(
      std::tuple{"+x", Point(5, 0.5, 0), Vector(-1, 0, 0), 4.0f, 6.0f},
      std::tuple{"-x", Point(-5, 0.5, 0), Vector(1, 0, 0), 4.0f, 6.0f},
      std::tuple{"+y", Point(0.5, 5, 0), Vector(0, -1, 0), 4.0f, 6.0f},
      std::tuple{"-y", Point(0.5, -5, 0), Vector(0, 1, 0), 4.0f, 6.0f},
      std::tuple{"+z", Point(0.5, 0, 5), Vector(0, 0, -1), 4.0f, 6.0f},
      std::tuple{"-z", Point(0.5, 0, -5), Vector(0, 0, 1), 4.0f, 6.0f},
      std::tuple{"inside", Point(0, 0.5, 0), Vector(0, 0, 1), -1.0f, 1.0f});

  SECTION(std::string("Direction ") + label) {
    const auto r = Ray(origin, direction);
    const auto c = std::make_shared<Cube>();
    const auto xs = c->intersept(r);
    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].t() == Approx(t1));
    REQUIRE(xs[1].t() == Approx(t2));
  }

  SECTION("ray misses the cube") {
    auto [origin, direction] =
        GENERATE(std::tuple{Point(-2, 0, 0), Vector(0.2673, 0.5345, 0.8018)},
                 std::tuple{Point(0, -2, 0), Vector(0.8018, 0.2673, 0.5345)},
                 std::tuple{Point(0, 0, -2), Vector(0.5345, 0.8018, 0.2673)},
                 std::tuple{Point(2, 0, 2), Vector(0, 0, -1)},
                 std::tuple{Point(0, 2, 2), Vector(0, -1, 0)},
                 std::tuple{Point(2, 2, 0), Vector(-1, 0, 0)});

    const auto r = Ray(origin, direction);
    const auto c = std::make_shared<Cube>();
    const auto xs = c->intersept(r);
    REQUIRE(xs.empty());
  }
}

TEST_CASE("cube - normalsAt()") {
  auto [point, expected_normal] =
      GENERATE(std::tuple{Point(1, 0.5, -0.8), Vector(1, 0, 0)},
               std::tuple{Point(-1, -0.2, 0.9), Vector(-1, 0, 0)},
               std::tuple{Point(-0.4, 1, -0.1), Vector(0, 1, 0)},
               std::tuple{Point(0.3, -1, -0.7), Vector(0, -1, 0)},
               std::tuple{Point(-0.6, 0.3, 1), Vector(0, 0, 1)},
               std::tuple{Point(0.4, 0.4, -1), Vector(0, 0, -1)},
               std::tuple{Point(1, 1, 1), Vector(1, 0, 0)},
               std::tuple{Point(-1, -1, -1), Vector(-1, 0, 0)});

  const auto cube = std::make_shared<Cube>();
  const auto normal = cube->normalsAt(point, Intersection{0.f, cube});
  REQUIRE(normal == expected_normal);
}
