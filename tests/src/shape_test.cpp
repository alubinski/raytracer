#include "intersection.h"
#include "material.h"
#include "ray.h"
#include "shape.h"
#include "transformations.h"
#include "tuple.h"
#include <catch2/catch.hpp>
#include <cmath>
#include <memory>
#include <optional>
#include <vector>

class TestShape : public Shape {
public:
  TestShape() : Shape(), localRay_(Ray(Point(0, 0, 0), Vector(0, 0, 1))) {}

  std::vector<Intersection> intersept(const Ray &ray) const override {
    localRay_ = ray;
    return {};
  }

  vector_t localNormalsAt(const point_t &p) const override {
    return Vector(p.x, p.y, p.z);
  }

  bool operator==(const Shape &other) const override { return false; }

  mutable Ray localRay_;
};

TEST_CASE("shape - transformation") {
  SECTION("default") {
    const auto s = std::make_shared<TestShape>();
    REQUIRE(s->transformation() == Mat44::identity());
  }

  SECTION("change shape transformation") {
    const auto s = std::make_shared<TestShape>();
    const auto t = translation(2, 3, 4);
    s->setTransformation(t);
    REQUIRE(s->transformation() == t);
  }
}

TEST_CASE("shape - default material") {
  const auto s = std::make_shared<TestShape>();
  const auto m = s->material();
  REQUIRE(m == Material());
}

TEST_CASE("shape - assign material") {
  auto s = std::make_shared<TestShape>();
  auto m = Material();
  m.setAmbient(1.f);
  s->setMaterial(m);
  REQUIRE(s->material() == m);
}

TEST_CASE("shape - intersept") {
  SECTION("scaled shape") {
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    auto s = std::make_shared<TestShape>();
    s->setTransformation(scaling(2, 2, 2));
    const auto xs = r.intersept(s);
    REQUIRE(s->localRay_.origin() == Point(0, 0, -2.5));
    REQUIRE(s->localRay_.direction() == Vector(0, 0, .5));
  }

  SECTION("translated shape") {
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    auto s = std::make_shared<TestShape>();
    s->setTransformation(translation(5, 0, 0));
    const auto xs = r.intersept(s);
    REQUIRE(s->localRay_.origin() == Point(-5, 0, -5));
    REQUIRE(s->localRay_.direction() == Vector(0, 0, 1));
  }
}

TEST_CASE("shape - normalsAt()") {
  SECTION("translated shape") {
    auto s = std::make_shared<TestShape>();
    s->setTransformation(translation(0, 1, 0));
    const auto n = s->normalsAt(Point(0, 1.70711, -0.70711));
    REQUIRE(n == Vector(0, 0.70711, -0.70711));
  }

  SECTION("transformed shape") {
    auto s = std::make_shared<TestShape>();
    s->setTransformation(scaling(1, .5, 1) * rotationZ(M_PI / 5));
    const auto n = s->normalsAt(Point(0, std::sqrt(2) / 2, -std::sqrt(2) / 2));
    REQUIRE(n == Vector(0, 0.97014, -0.24254));
  }
}
