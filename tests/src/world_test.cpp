#include "color.h"
#include "intersection.h"
#include "lightning.h"
#include "ray.h"
#include "sphere.h"
#include "transformations.h"
#include "tuple.h"
#include "world.h"
#include <catch2/catch.hpp>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

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
