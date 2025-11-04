#include "camera.h"
#include "color.h"
#include "matrix.h"
#include "transformations.h"
#include "tuple.h"
#include <catch2/catch.hpp>
#include <cmath>
#include <cstdint>
#include <iostream>

TEST_CASE("camera - constructor") {
  const uint32_t hsize = 160;
  const uint32_t vsize = 140;
  const float fieldOfview = M_PI / 2;
  const auto c = Camera(hsize, vsize, fieldOfview);
  REQUIRE(c.hsize() == hsize);
  REQUIRE(c.vsize() == vsize);
  REQUIRE(c.fieldOfView() == Approx(fieldOfview));
  REQUIRE(c.transform() == Mat44::identity());
}

TEST_CASE("camera - pixelSize()") {
  SECTION("horizontal canvas") {
    const auto c = Camera(200, 125, M_PI / 2);
    REQUIRE(c.pixelSize() == Approx(0.01f));
  }

  SECTION("verical canvas") {
    const auto c = Camera(125, 200, M_PI / 2);
    REQUIRE(c.pixelSize() == Approx(0.01f));
  }
}

TEST_CASE("camera - rayForPixel()") {
  SECTION("Constructing a ray through the center of the canvas") {
    const auto c = Camera(201, 101, M_PI / 2);
    const auto r = c.rayForPixel(100, 50);
    REQUIRE(r.origin() == Point(0, 0, 0));
    REQUIRE(r.direction() == Vector(0, 0, -1));
  }

  SECTION("Constructing a ray through a corner of the canvas") {
    const auto c = Camera(201, 101, M_PI / 2);
    const auto r = c.rayForPixel(0, 0);
    REQUIRE(r.origin() == Point(0, 0, 0));
    REQUIRE(r.direction() == Vector(0.66519, 0.33259, -0.66851));
  }

  SECTION("Constructing a ray when the camera is transformed") {
    auto c = Camera(201, 101, M_PI / 2);
    c.transform() = rotationY(M_PI / 4) * translation(0, -2, 5);
    const auto r = c.rayForPixel(100, 50);
    REQUIRE(r.origin() == Point(0, 2, -5));
    REQUIRE(r.direction() == Vector(std::sqrt(2) / 2, 0, -std::sqrt(2) / 2));
  }
}

TEST_CASE("camera - render()") {
  const auto w = World::defaultWorld();
  auto c = Camera(11, 11, M_PI / 2);
  const auto from = Point(0, 0, -5);
  const auto to = Point(0, 0, 0);
  const auto up = Vector(0, 1, 0);
  // c.transform() = view(from, to, up);
  c.setTransorm(view(from, to, up));

  // std::cout << c.transform() << std::endl;
  const auto image = c.render(w);
  const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  const auto color = w.colorAt(r);
  std::cout << color << std::endl;

  // const auto ray = c.rayForPixel(5, 5);
  // const auto color = w.colorAt(ray);
  // std::cout << c.transform() << std::endl;
  REQUIRE(image(5, 5) == Color(0.38066, 0.47583, 0.2855));
}
