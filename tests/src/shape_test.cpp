#include "intersection.h"
#include "shape.h"
#include "transformations.h"
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
