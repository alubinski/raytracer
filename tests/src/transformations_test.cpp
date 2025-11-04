#include "matrix.h"
#include "matrix_impl.h"
#include "transformations.h"
#include "tuple.h"
#include <catch2/catch.hpp>
#include <cmath>

TEST_CASE("translation matrix") {
  SECTION("translation_matrix * point") {
    const auto transform = translation(5, -3, 2);
    const auto p = Point(-3, 4, 5);
    REQUIRE(transform * p == Point(2, 1, 7));
  }

  SECTION("translation_matrix.inverse * point") {
    const auto inv = translation(5, -3, 2).inverse();
    const auto p = Point(-3, 4, 5);
    REQUIRE(inv * p == Point(-8, 7, 3));
  }

  SECTION("translation_matrix * vector") {
    const auto transform = translation(5, -3, 2);
    const auto v = Vector(-3, 4, 5);
    REQUIRE(transform * v == v);
  }
}

TEST_CASE("scaling matrix") {
  SECTION("scaling_matrix * point") {
    const auto transform = scaling(2, 3, 4);
    const auto p = Point(-4, 6, 8);
    REQUIRE(transform * p == Point(-8, 18, 32));
  }

  SECTION("scaling_matrix * vector") {
    const auto transform = scaling(2, 3, 4);
    const auto v = Vector(-4, 6, 8);
    REQUIRE(transform * v == Vector(-8, 18, 32));
  }

  SECTION("scaling_matrix.inverse * tuple") {
    const auto transform = scaling(2, 3, 4);
    const auto inv = transform.inverse();
    const auto v = Vector(-4, 6, 8);
    REQUIRE(inv * v == Vector(-2, 2, 2));
  }

  SECTION("reflection") {
    const auto transform = scaling(-1, 1, 1);
    const auto p = Point(2, 3, 4);
    REQUIRE(transform * p == Point(-2, 3, 4));
  }
}

TEST_CASE("x rotation matrix") {
  SECTION("rotationX_matrix * point") {
    const auto p = Point(0, 1, 0);
    const auto half_quarter = rotationX(M_PI / 4);
    const auto full_quarter = rotationX(M_PI / 2);
    REQUIRE(half_quarter * p == Point(0, std::sqrt(2) / 2, std::sqrt(2) / 2));
    REQUIRE(full_quarter * p == Point(0, 0, 1));
  }

  SECTION("rotationX_matrix.inverse * point") {
    const auto p = Point(0, 1, 0);
    const auto half_quarter = rotationX(M_PI / 4);
    const auto inv = half_quarter.inverse();
    REQUIRE(inv * p == Point(0, std::sqrt(2) / 2, -std::sqrt(2) / 2));
  }
}

TEST_CASE("y rotation matrix") {
  SECTION("rotationY_matrix * point") {
    const auto p = Point(0, 0, 1);
    const auto half_quarter = rotationY(M_PI / 4);
    const auto full_quarter = rotationY(M_PI / 2);
    REQUIRE(half_quarter * p == Point(std::sqrt(2) / 2, 0, std::sqrt(2) / 2));
    REQUIRE(full_quarter * p == Point(1, 0, 0));
  }
}

TEST_CASE("z rotation matrix") {
  SECTION("rotationZ_matrix * point") {
    const auto p = Point(0, 1, 0);
    const auto half_quarter = rotationZ(M_PI / 4);
    const auto full_quarter = rotationZ(M_PI / 2);
    REQUIRE(half_quarter * p == Point(-std::sqrt(2) / 2, std::sqrt(2) / 2, 0));
    REQUIRE(full_quarter * p == Point(-1, 0, 0));
  }
}

TEST_CASE("shearing matrix") {
  SECTION("xy") {
    const auto transform = shearing(1, 0, 0, 0, 0, 0);
    const auto p = Point(2, 3, 4);
    REQUIRE(transform * p == Point(5, 3, 4));
  }

  SECTION("xz") {
    const auto transform = shearing(0, 1, 0, 0, 0, 0);
    const auto p = Point(2, 3, 4);
    REQUIRE(transform * p == Point(6, 3, 4));
  }

  SECTION("yx") {
    const auto transform = shearing(0, 0, 1, 0, 0, 0);
    const auto p = Point(2, 3, 4);
    REQUIRE(transform * p == Point(2, 5, 4));
  }

  SECTION("yz") {
    const auto transform = shearing(0, 0, 0, 1, 0, 0);
    const auto p = Point(2, 3, 4);
    REQUIRE(transform * p == Point(2, 7, 4));
  }

  SECTION("zx") {
    const auto transform = shearing(0, 0, 0, 0, 1, 0);
    const auto p = Point(2, 3, 4);
    REQUIRE(transform * p == Point(2, 3, 6));
  }

  SECTION("zy") {
    const auto transform = shearing(0, 0, 0, 0, 0, 1);
    const auto p = Point(2, 3, 4);
    REQUIRE(transform * p == Point(2, 3, 7));
  }
}

TEST_CASE("complex transformations") {
  SECTION("applied in sequence") {
    const auto p = Point(1, 0, 1);
    const auto A = rotationX(M_PI / 2);
    const auto B = scaling(5, 5, 5);
    const auto C = translation(10, 5, 7);

    const auto p2 = A * p;
    REQUIRE(p2 == Point(1, -1, 0));

    const auto p3 = B * p2;
    REQUIRE(p3 == Point(5, -5, 0));

    const auto p4 = C * p3;
    REQUIRE(p4 == Point(15, 0, 7));
  }

  SECTION("applied chained") {
    const auto p = Point(1, 0, 1);
    const auto A = rotationX(M_PI / 2);
    const auto B = scaling(5, 5, 5);
    const auto C = translation(10, 5, 7);

    const auto T = C * B * A;
    REQUIRE(T * p == Point(15, 0, 7));
  }

  SECTION("using matrix API") {
    const auto p = Point(1, 0, 1);
    const auto A = rotationX(M_PI / 2);
    const auto B = scaling(5, 5, 5);
    const auto C = translation(10, 5, 7);

    const auto T = rotationX(M_PI / 2).scale(5, 5, 5).translate(10, 5, 7);
    REQUIRE(T * p == Point(15, 0, 7));
  }
}

TEST_CASE("view matrix") {
  SECTION("default orientation") {
    const auto from = Point(0, 0, 0);
    const auto to = Point(0, 0, -1);
    const auto up = Vector(0, 1, 0);
    const auto t = view(from, to, up);
    REQUIRE(t == Mat44::identity());
  }

  SECTION("looking in positive z direction") {
    const auto from = Point(0, 0, 0);
    const auto to = Point(0, 0, 1);
    const auto up = Vector(0, 1, 0);
    const auto t = view(from, to, up);
    REQUIRE(t == scaling(-1, 1, -1));
  }

  SECTION("view transformation moves the world") {
    const auto from = Point(0, 0, 8);
    const auto to = Point(0, 0, 0);
    const auto up = Vector(0, 1, 0);
    const auto t = view(from, to, up);
    REQUIRE(t == translation(0, 0, -8));
  }

  SECTION("arbritary orientation") {
    const auto from = Point(1, 3, 2);
    const auto to = Point(4, -2, 8);
    const auto up = Vector(1, 1, 0);
    const auto t = view(from, to, up);
    REQUIRE(t == Mat44{{-0.50709, 0.50709, 0.67612, -2.36643},
                       {0.76772, 0.60609, 0.12122, -2.82843},
                       {-0.35857, 0.59761, -0.71714, 0.00000},
                       {0.00000, 0.00000, 0.00000, 1.00000}});
  }
}
