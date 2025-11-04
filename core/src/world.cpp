#include "world.h"
#include "color.h"
#include "intersection.h"
#include "lightning.h"
#include "ray.h"
#include "sphere.h"
#include "transformations.h"
#include "tuple.h"
#include <iostream>

void World::addObject(const ShapePtr &object) { objects_.push_back(object); }

void World::removeObject(const ShapePtr &object) {
  objects_.erase(std::remove(objects_.begin(), objects_.end(), object),
                 objects_.end());
}

bool World::containsObject(const ShapePtr &object) const {
  if (!object)
    return false;
  for (const auto &obj : objects_) {
    if (!obj)
      continue;
    if (obj->operator==(*object)) { // explicitly call operator==
      return true;
    }
  }
  return false;
}
void World::addLight(const PointLight &light) { lights_.push_back(light); }

void World::removeLight(const PointLight &light) {
  lights_.erase(std::remove(lights_.begin(), lights_.end(), light),
                lights_.end());
}

bool World::containsLight(const PointLight &light) const {
  return std::find(lights_.begin(), lights_.end(), light) != lights_.end();
}

void World::clear() {
  objects_.clear();
  lights_.clear();
}

bool World::isEmpty() const { return objects_.empty() && lights_.empty(); }

World World::defaultWorld() {
  World w;

  PointLight light(Point(-10, 10, -10), Color(1, 1, 1));
  w.addLight(light);

  auto s1 = std::make_shared<Sphere>();
  s1->material().setColor(Color(0.8, 1.0, 0.6));
  s1->material().setDiffuse(0.7f);
  s1->material().setSpecular(0.2f);

  auto s2 = std::make_shared<Sphere>();
  s2->setTransformation(scaling(0.5, 0.5, 0.5));

  w.addObject(s1);
  w.addObject(s2);

  return w;
}

Color World::shadeHit(const ComputationData &comps) const {
  Color result{0, 0, 0};
  for (const auto &light : lights_) {
    result = result + lightining(comps.object->material(), light, comps.point,
                                 comps.eyeV, comps.normalV);
  }
  return result;
}

Color World::colorAt(const Ray &r) const {
  const auto xs = r.intersept(*this);
  const auto h = hit(xs);
  if (!h.has_value()) {
    return Color(0, 0, 0);
  }
  const auto comps = r.precompute(h.value());
  return shadeHit(comps);
}
