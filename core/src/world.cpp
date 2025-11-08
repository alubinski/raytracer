#include "world.h"
#include "color.h"
#include "computation.h"
#include "intersection.h"
#include "lightning.h"
#include "ray.h"
#include "sphere.h"
#include "transformations.h"
#include "tuple.h"
#include <cmath>
#include <cstdint>
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

Color World::shadeHit(const ComputationData &comps,
                      uint8_t recursion_limit) const {
  Color result{0, 0, 0};
  for (const auto &light : lights_) {
    bool shadowed = isShadowed(comps.overPoint);

    Color surface =
        lightining(comps.object->material(), comps.object, light,
                   comps.overPoint, comps.eyeV, comps.normalV, shadowed);

    Color reflected = reflectedColor(comps, recursion_limit);
    Color refracted = reflactedColor(comps, recursion_limit);
    const Material &material = comps.object->material();

    if (material.reflective() > 0.f && material.transparency() > 0.f) {
      float reflectance = schlick(comps);
      result = result + surface + reflected * reflectance +
               refracted * (1.f - reflectance);
    } else {
      result = result + surface + reflected + refracted;
    }
  }
  return result;
}

Color World::reflectedColor(const ComputationData &comps,
                            uint8_t recursion_limit) const {
  if (comps.object->material().reflective() == 0.f || recursion_limit == 0) {
    return Color(0, 0, 0);
  }

  const Ray reflectRay{comps.overPoint, comps.reflectiveV};
  return colorAt(reflectRay, --recursion_limit) *
         comps.object->material().reflective();
}

Color World::reflactedColor(const ComputationData &comps,
                            uint8_t recursionLimit) const {
  if (comps.object->material().transparency() == 0.f || recursionLimit == 0) {
    return Color::black();
  }
  const float n_ratio = comps.n1 / comps.n2;
  const float cos_i = dotProduct(comps.eyeV, comps.normalV);

  const float sin2_t = n_ratio * n_ratio * (1.f - cos_i * cos_i);
  if (sin2_t > 1.f) {
    return Color::black();
  }

  const float cos_t = std::sqrt(1.0f - sin2_t);
  const vector_t direction =
      comps.normalV * (n_ratio * cos_i - cos_t) - comps.eyeV * n_ratio;

  const Ray reflectedRay{comps.underPoint, direction};
  return colorAt(reflectedRay, recursionLimit - 1) *
         comps.object->material().transparency();
}

Color World::colorAt(const Ray &r, uint8_t recursion_limit) const {
  const auto xs = r.intersept(*this);
  const auto h = hit(xs);
  if (!h.has_value()) {
    return Color(0, 0, 0);
  }
  const auto comps = r.precompute(h.value(), xs);
  return shadeHit(comps, recursion_limit);
}

bool World::isShadowed(point_t point) const {
  // TO-DO: support multiple light source
  const auto v = lights_[0].position() - point;

  const auto distance = v.magnitude();
  const auto direction = v.normalize();

  const Ray r{point, direction};
  const auto xs = r.intersept(*this);
  const auto h = hit(xs);
  if (h && h->t() < distance) {
    return true;
  }
  return false;
}
