#include "group.h"
#include "intersection.h"
#include "matrix.h"
#include "ray.h"
#include "transformations.h"
#include "tuple.h"
#include <catch2/catch.hpp>
#include <memory>

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

TEST_CASE("group - constructor") {
  const auto g = Group();
  REQUIRE(g.transformation() == Mat44::identity());
  REQUIRE(g.empty());
}

TEST_CASE("group - add()") {
  auto g = std::make_shared<Group>();
  const auto s = std::make_shared<TestShape>();
  g->add(s);
  REQUIRE_FALSE(g->empty());
  REQUIRE(g->includes(s));
  REQUIRE(s->parent() == g);
}

TEST_CASE("group - intersept()") {
  SECTION("empty group") {
    const auto g = std::make_shared<Group>();
    const auto r = Ray{Point(0, 0, 0), Vector(0, 0, 1)};
    const auto xs = g->intersept(r);
    REQUIRE(xs.empty());
  }

  SECTION("non-empty group") {
    auto g = std::make_shared<Group>();
    auto s1 = std::make_shared<Sphere>();
    auto s2 = std::make_shared<Sphere>();
    auto s3 = std::make_shared<Sphere>();
    s2->transformation() = translation(0, 0, -3);
    s3->transformation() = translation(5, 0, 0);
    g->add(s1);
    g->add(s2);
    g->add(s3);
    const auto r = Ray{Point(0, 0, -5), Vector(0, 0, 1)};
    const auto xs = g->intersept(r);

    REQUIRE(xs.size() == 4);
    REQUIRE(xs[0].object() == s2);
    REQUIRE(xs[1].object() == s2);
    REQUIRE(xs[2].object() == s1);
    REQUIRE(xs[3].object() == s1);
  }

  SECTION("transformed group") {
    auto g = std::make_shared<Group>();
    g->transformation() = scaling(2, 2, 2);
    auto s = std::make_shared<Sphere>();
    s->transformation() = translation(5, 0, 0);
    g->add(s);
    const auto r = Ray{Point(10, 0, -10), Vector(0, 0, 1)};
    const auto xs = g->intersept(r);
    REQUIRE(xs.size() == 2);
  }
}
