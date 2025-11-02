#include "intersection.h"
#include "material.h"
#include "shape.h"
#include "transformations.h"
#include "tuple.h"
#include <catch2/catch.hpp>
#include <memory>
#include <optional>

class TestShape : public Shape {
public:
  TestShape() : Shape() {}
  std::optional<std::pair<Intersection, Intersection>>
  intersept(const Ray &ray) const {
    return std::nullopt;
  }

  vector_t normalsAt(const point_t &p) const { return Vector(0, 0, 0); }
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
