
#include "tuple.h"
#include <catch2/catch.hpp>
#include <cmath>

TEST_CASE("tuple - Initialization") {
  SECTION("A tuple with w=1.0 is a point") {
    const auto t = Tuple(4.3, -4.2, 3.1, 1.0);
    REQUIRE(t.x == Approx(4.3));
    REQUIRE(t.y == Approx(-4.2));
    REQUIRE(t.z == Approx(3.1));
    REQUIRE(t.isPoint());
    REQUIRE_FALSE(t.isVector());
  }

  SECTION("A tuple with w=0 is a vector") {
    const auto t = Tuple(4.3, -4.2, 3.1, 0.0);
    REQUIRE(t.x == Approx(4.3));
    REQUIRE(t.y == Approx(-4.2));
    REQUIRE(t.z == Approx(3.1));
    REQUIRE_FALSE(t.isPoint());
    REQUIRE(t.isVector());
  }

  SECTION("Point() creates tuples with w=1") {
    const auto p = Point(4, -4, 3);
    REQUIRE(p == Tuple(4, -4, 3, 1));
  }

  SECTION("Vector() creates tuples with w=0") {
    const auto v = Vector(4, -4, 3);
    REQUIRE(v == Tuple(4, -4, 3, 0));
  }
}

TEST_CASE("tuple - Addition operator") {
  SECTION("tuple + tuple") {
    const auto t1 = Tuple(3, -2, 5, 1);
    const auto t2 = Tuple(-2, 3, 1, 0);
    REQUIRE(t1 + t2 == Tuple(1, 1, 6, 1));
  }
}

TEST_CASE("tuple - Subtraction operator") {
  SECTION("tuple - tuple") {
    const auto p1 = Point(3, 2, 1);
    const auto p2 = Point(5, 6, 7);
    REQUIRE(p1 - p2 == Vector(-2, -4, -6));
  }

  SECTION("point - vector") {
    const auto p = Point(3, 2, 1);
    const auto v = Vector(5, 6, 7);
    REQUIRE(p - v == Point(-2, -4, -6));
  }

  SECTION("vector - vector") {
    const auto v1 = Vector(3, 2, 1);
    const auto v2 = Vector(5, 6, 7);
    REQUIRE(v1 - v2 == Vector(-2, -4, -6));
  }

  SECTION("Subtracting a vector from the zero vector") {
    const auto zero = Vector(0, 0, 0);
    const auto v = Vector(1, -2, 3);
    REQUIRE(zero - v == Vector(-1, 2, -3));
  }
}

TEST_CASE("tuple - Unary minus operator") {
  const auto t = Tuple(1, -2, 3, -4);
  REQUIRE(-t == Tuple(-1, 2, -3, 4));
}

TEST_CASE("tuple - Multiplication operator") {
  SECTION("tuple * scalar") {
    const auto t = Tuple(1, -2, 3, -4);
    REQUIRE(t * 3.5 == Tuple(3.5, -7, 10.5, -14));
  }

  SECTION("tuple * fraction") {
    const auto t = Tuple(1, -2, 3, -4);
    REQUIRE(t * 0.5 == Tuple(0.5, -1, 1.5, -2));
  }
}

TEST_CASE("tuple - Division operator") {
  SECTION("tuple / scalar") {
    const auto t = Tuple(1, -2, 3, -4);
    REQUIRE(t / 2 == Tuple(0.5, -1, 1.5, -2));
  }
}

TEST_CASE("tuple - Magnitude") {
  SECTION("v = [0, 1, 0]") {
    const auto v = Vector(0, 1, 0);
    REQUIRE(v.magnitude() == Approx(1));
  }

  SECTION("v = [0, 0, 1]") {
    const auto v = Vector(0, 0, 1);
    REQUIRE(v.magnitude() == Approx(1));
  }

  SECTION("v = [1, 2, 3]") {
    const auto v = Vector(1, 2, 3);
    REQUIRE(v.magnitude() == Approx(std::sqrt(14)));
  }

  SECTION("v = [-1, -2, -3]") {
    const auto v = Vector(-1, -2, -3);
    REQUIRE(v.magnitude() == Approx(std::sqrt(14)));
  }
}

TEST_CASE("tuple - Normalize") {
  SECTION("v = [4, 0, 0]") {
    const auto v = Vector(4, 0, 0);
    REQUIRE(v.normalize() == Vector(1, 0, 0));
  }

  SECTION("v = [1, 2, 3]") {
    const auto v = Vector(1, 2, 3);
    REQUIRE(v.normalize() ==
            Vector(1 / std::sqrt(14), 2 / std::sqrt(14), 3 / std::sqrt(14)));
  }

  SECTION("Magnitude of normalized vector") {
    const auto v = Vector(1, 2, 3);
    REQUIRE(v.normalize().magnitude() == Approx(1.0f));
  }
}

TEST_CASE("tuple - Dot product") {
  const auto v1 = Vector(1, 2, 3);
  const auto v2 = Vector(2, 3, 4);
  REQUIRE(dotProduct(v1, v2) == Approx(20.0f));
}

TEST_CASE("tuple - Cross product") {
  const auto v1 = Vector(1, 2, 3);
  const auto v2 = Vector(2, 3, 4);
  REQUIRE(crossProduct(v1, v2) == Vector(-1, 2, -1));
  REQUIRE(crossProduct(v2, v1) == Vector(1, -2, 1));
}
