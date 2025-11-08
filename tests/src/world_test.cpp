#include "color.h"
#include "intersection.h"
#include "lightning.h"
#include "plane.h"
#include "ray.h"
#include "sphere.h"
#include "transformations.h"
#include "tuple.h"
#include "world.h"
#include <catch2/catch.hpp>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

class TestPattern : public Pattern {
  Color colorAt(const point_t &point) const override {
    return Color(point.x, point.y, point.z);
  }
  bool operator==(PatternPtr const &other) const override {
    return std::dynamic_pointer_cast<TestPattern>(other) != nullptr;
  }
};

TEST_CASE("world - default constructor") {
  const auto w = World();
  REQUIRE(w.objects().empty());
  REQUIRE(w.lights().empty());
}

TEST_CASE("world - defaultWorld()") {
  const auto light = PointLight(Point(-10, 10, -10), Color(1, 1, 1));
  auto s1 = std::make_shared<Sphere>();
  s1->material().setColor(Color(0.8, 1.0, 0.6));
  s1->material().setDiffuse(0.7f);
  s1->material().setSpecular(0.2f);

  auto s2 = std::make_shared<Sphere>();
  s2->setTransformation(scaling(0.5, 0.5, 0.5));

  const auto w = World::defaultWorld();
  REQUIRE(w.lights().size() == 1);
  REQUIRE(w.lights()[0] == light);
  REQUIRE(w.objects().size() == 2);
  REQUIRE(w.containsObject(s1));
  REQUIRE(w.containsObject(s2));
}

TEST_CASE("world - ray.intersect()") {
  const auto w = World::defaultWorld();
  const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  const auto xs = r.intersept(w);
  REQUIRE(xs.size() == 4);
  REQUIRE(xs[0].t() == Approx(4.f));
  REQUIRE(xs[1].t() == Approx(4.5f));
  REQUIRE(xs[2].t() == Approx(5.5f));
  REQUIRE(xs[3].t() == Approx(6.f));
}

TEST_CASE("world - shadeHit()") {
  SECTION("from the outside") {
    const auto w = World::defaultWorld();
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    const auto &shape = w.objects()[0];
    const auto i = Intersection(4, shape);
    const auto comps = r.precompute(i);
    const auto c = w.shadeHit(comps);
    REQUIRE(c == Color(0.38066, 0.47583, 0.2855));
  }

  SECTION("from the inside") {
    auto w = World::defaultWorld();
    w.lights()[0] = PointLight(Point(0, 0.25, 0), Color(1, 1, 1));
    const auto r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
    const auto &shape = w.objects()[1];
    const auto i = Intersection(0.5, shape);
    const auto comps = r.precompute(i);
    const auto c = w.shadeHit(comps);
    REQUIRE(c == Color(0.90498, 0.90498, 0.90498));
  }

  SECTION("intersection in shadow") {
    auto w = World();
    w.addLight(PointLight(Point(0, 0, -10), Color(1, 1, 1)));
    const auto s1 = std::make_shared<Sphere>();
    w.addObject(s1);
    auto s2 = std::make_shared<Sphere>();
    s2->setTransformation(translation(0, 0, 10));
    w.addObject(s2);
    const auto r = Ray(Point(0, 0, 5), Vector(0, 0, 1));
    const auto i = Intersection(4, s2);
    const auto comps = r.precompute(i);
    const auto c = w.shadeHit(comps);
    REQUIRE(c == Color(0.1, 0.1, 0.1));
  }

  SECTION("with reflective material") {
    auto w = World::defaultWorld();
    auto shape = std::make_shared<Plane>();
    shape->material().setReflective(0.5f);
    shape->transformation() = translation(0, -1, 0);
    w.addObject(shape);
    const auto r =
        Ray(Point(0, 0, -3), Vector(0, -std::sqrt(2) / 2, std::sqrt(2) / 2));
    const auto i = Intersection(std::sqrt(2), shape);
    const auto comps = r.precompute(i);
    const auto color = w.shadeHit(comps);
    REQUIRE(color == Color(0.87677, 0.92436, 0.82918));
  }

  SECTION("with transpalent material") {
    auto w = World::defaultWorld();
    auto floor = std::make_shared<Plane>();
    floor->transformation() = translation(0, -1, 0);
    floor->material().setTransparency(.5f);
    floor->material().setReflectiveIndex(1.5f);
    w.addObject(floor);
    auto ball = std::make_shared<Sphere>();
    ball->material().setColor(Color(1, 0, 0));
    ball->material().setAmbient(0.5);
    ball->transformation() = translation(0, -3.5, -0.5);
    w.addObject(ball);
    const auto r =
        Ray(Point(0, 0, -3), Vector(0, -std::sqrt(2) / 2, std::sqrt(2) / 2));
    const auto xs = intersections(Intersection(std::sqrt(2), floor));
    const auto comps = r.precompute(xs[0], xs);
    const auto color = w.shadeHit(comps);
    REQUIRE(color == Color(0.93642, 0.68642, 0.68642));
  }

  SECTION("with a reflective, transparent material") {
    auto w = World::defaultWorld();
    const auto r = Ray(Point(0, 0, -3), Vector(0, -SQRT_2 / 2, SQRT_2 / 2));
    auto floor = std::make_shared<Plane>();
    floor->transformation() = translation(0, -1, 0);
    floor->material().setReflective(.5f);
    floor->material().setTransparency(.5f);
    floor->material().setReflectiveIndex(1.5f);
    w.addObject(floor);
    auto ball = std::make_shared<Sphere>();
    ball->material().setColor(Color(1, 0, 0));
    ball->material().setAmbient(0.5);
    ball->transformation() = translation(0, -3.5, -0.5);
    w.addObject(ball);
    const auto xs = intersections(Intersection(SQRT_2, floor));
    const auto comps = r.precompute(xs[0], xs);
    const auto color = w.shadeHit(comps, 5);
    REQUIRE(color == Color(0.93391, 0.69643, 0.69243));
  }
}

TEST_CASE("world - colorAt()") {
  SECTION("ray misses") {
    const auto w = World::defaultWorld();
    const auto r = Ray(Point(0, 0, -5), Vector(0, 1, 0));
    const auto c = w.colorAt(r);
    REQUIRE(c == Color(0, 0, 0));
  }

  SECTION("ray hits") {
    const auto w = World::defaultWorld();
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    const auto c = w.colorAt(r);
    REQUIRE(c == Color(0.38066, 0.47583, 0.2855));
  }

  SECTION("intersection beind the ray") {
    auto w = World::defaultWorld();
    auto outer = w.objects()[0];
    outer->material().setAmbient(1);
    auto inner = w.objects()[1];
    inner->material().setAmbient(1);
    const auto r = Ray(Point(0, 0, 0.75), Vector(0, 0, -1));
    const auto c = w.colorAt(r);
    REQUIRE(c == inner->material().color());
  }

  SECTION("colorAt() terminates succesfully") {
    auto w = World();
    w.addLight(PointLight(Point(0, 0, 0), Color(1, 1, 1)));
    auto lower = std::make_shared<Plane>();
    lower->material().setReflective(1);
    lower->transformation() = translation(0, -1, 0);
    auto upper = std::make_shared<Plane>();
    upper->material().setReflective(1);
    upper->transformation() = translation(0, 1, 0);
    w.addObject(lower);
    w.addObject(upper);
    const auto r = Ray(Point(0, 0, 1), Vector(0, 1, 0));
    REQUIRE(w.colorAt(r) != Color::black());
  }
}

TEST_CASE("world - isShadowed()") {
  SECTION("object not collinear with point nor light") {
    const auto w = World::defaultWorld();
    const auto p = Point(0, 10, 0);
    REQUIRE_FALSE(w.isShadowed(p));
  }

  SECTION("object between point and light") {
    const auto w = World::defaultWorld();
    const auto p = Point(10, -10, 10);
    REQUIRE(w.isShadowed(p));
  }

  SECTION("object behind the light") {
    const auto w = World::defaultWorld();
    const auto p = Point(-20, 20, -20);
    REQUIRE_FALSE(w.isShadowed(p));
  }

  SECTION("point between object and light") {
    const auto w = World::defaultWorld();
    const auto p = Point(-2, 2, -2);
    REQUIRE_FALSE(w.isShadowed(p));
  }
}

TEST_CASE("world - reflectedColor()") {
  SECTION("nonreflective material") {
    auto w = World::defaultWorld();
    auto shape = w.objects()[1];
    shape->material().setAmbient(1.f);
    const auto r = Ray(Point(0, 0, 1), Vector(0, 0, 1));
    const auto i = Intersection(1, shape);
    const auto comps = r.precompute(i);
    const auto color = w.reflectedColor(comps);
    REQUIRE(color == Color(0, 0, 0));
  }

  SECTION("reflective material") {
    auto w = World::defaultWorld();
    auto shape = std::make_shared<Plane>();
    shape->material().setReflective(0.5f);
    shape->transformation() = translation(0, -1, 0);
    w.addObject(shape);
    const auto r =
        Ray(Point(0, 0, -3), Vector(0, -std::sqrt(2) / 2, std::sqrt(2) / 2));
    const auto i = Intersection(std::sqrt(2), shape);
    const auto comps = r.precompute(i);
    const auto color = w.reflectedColor(comps);
    REQUIRE(color == Color(0.19032, 0.2379, 0.14274));
  }

  SECTION("recursion limited") {
    auto w = World::defaultWorld();
    auto shape = std::make_shared<Plane>();
    shape->material().setReflective(0.5f);
    shape->transformation() = translation(0, -1, 0);
    w.addObject(shape);
    const auto r =
        Ray(Point(0, 0, -3), Vector(0, -std::sqrt(2) / 2, std::sqrt(2) / 2));
    const auto i = Intersection(std::sqrt(2), shape);
    const auto comps = r.precompute(i);
    const auto color = w.reflectedColor(comps, 0);
    REQUIRE(color == Color::black());
  }
}

TEST_CASE("world - reflactedColor()") {
  SECTION("opaque surface") {
    const auto w = World::defaultWorld();
    const auto shape = w.objects()[0];
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    const auto xs =
        intersections(Intersection{4, shape}, Intersection{6, shape});
    const auto comps = r.precompute(xs[0], xs);
    const auto c = w.reflactedColor(comps, 5);
    REQUIRE(c == Color::black());
  }

  SECTION("maximum recursive depth") {
    auto w = World::defaultWorld();
    auto &shape = w.objects()[0];
    shape->material().setTransparency(1.0f);
    shape->material().setReflectiveIndex(1.5f);
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    const auto xs =
        intersections(Intersection{4, shape}, Intersection{6, shape});
    const auto comps = r.precompute(xs[0], xs);
    const auto c = w.reflactedColor(comps, 0);
    REQUIRE(c == Color::black());
  }

  SECTION("under total internal reflection") {
    auto w = World::defaultWorld();
    auto &shape = w.objects()[0];
    shape->material().setTransparency(1.0f);
    shape->material().setReflectiveIndex(1.5f);
    const auto r = Ray(Point(0, 0, std::sqrt(2) / 2), Vector(0, 1, 0));
    const auto xs = intersections(
        Intersection{static_cast<float>(-std::sqrt(2) / 2), shape},
        Intersection{static_cast<float>(std::sqrt(2) / 2), shape});
    const auto comps = r.precompute(xs[1], xs);
    const auto c = w.reflactedColor(comps, 5);
    REQUIRE(c == Color::black());
  }

  SECTION("with reflacted ray") {
    auto w = World::defaultWorld();
    auto &A = w.objects()[0];
    A->material().setAmbient(1.0f);
    const auto pattern = std::make_shared<TestPattern>();
    A->material().setPattern(pattern);
    auto &B = w.objects()[1];
    B->material().setTransparency(1.f);
    B->material().setReflectiveIndex(1.5f);
    const auto r = Ray(Point(0, 0, 0.1), Vector(0, 1, 0));
    const auto xs =
        intersections(Intersection(-0.9899, A), Intersection(-0.4899, B),
                      Intersection(0.4899, B), Intersection(0.9899, A));
    const auto comps = r.precompute(xs[2], xs);
    const auto c = w.reflactedColor(comps, 5);
    REQUIRE(c == Color(0, 0.99888, 0.04725));
  }
}
