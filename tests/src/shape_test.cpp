#include "group.h"
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

  SECTION("child object") {
    auto g1 = std::make_shared<Group>();
    g1->transformation() = rotationY(M_PI / 2);
    auto g2 = std::make_shared<Group>();
    g2->transformation() = scaling(1, 2, 3);
    g1->add(g2);
    auto s = std::make_shared<Sphere>();
    s->transformation() = translation(5, 0, 0);
    g2->add(s);
    const auto n = s->normalsAt(Point(1.7321, 1.1547, -5.5774));
    REQUIRE(n == Vector(0.2857, 0.4286, -0.8571));
  }
}

TEST_CASE("shape - parent attribute") {
  auto s = std::make_shared<TestShape>();

  REQUIRE(s->parent() == nullptr);
}

TEST_CASE("shape - worldPointToObjectPoint()") {
  auto g1 = std::make_shared<Group>();
  g1->transformation() = rotationY(M_PI / 2);
  auto g2 = std::make_shared<Group>();
  g2->transformation() = scaling(2, 2, 2);
  g1->add(g2);
  auto s = std::make_shared<Sphere>();
  s->transformation() = translation(5, 0, 0);
  g2->add(s);
  const auto p = s->worldPointToObjectPoint(Point(-2, 0, -10));
  REQUIRE(p == Point(0, 0, -1));
}

TEST_CASE("shape - objectNormalToWorldNormal()") {
  auto g1 = std::make_shared<Group>();
  g1->transformation() = rotationY(M_PI / 2);
  auto g2 = std::make_shared<Group>();
  g2->transformation() = scaling(1, 2, 3);
  g1->add(g2);
  auto s = std::make_shared<Sphere>();
  s->transformation() = translation(5, 0, 0);
  g2->add(s);
  const auto n =
      s->objectNormalToWorldNormal(Vector(SQRT_3 / 3, SQRT_3 / 3, SQRT_3 / 3));
  REQUIRE(n == Vector(0.2857, 0.4286, -0.8571));
}
