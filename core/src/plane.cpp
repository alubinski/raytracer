#include "plane.h"
#include "intersection.h"
#include "tuple.h"
#include <cmath>
#include <cstdlib>
#include <vector>

std::vector<Intersection> Plane::intersept(const Ray &ray) const {
  if (std::abs(ray.direction().y) < epsilon) {
    return {};
  }
  const float t = -ray.origin().y / ray.direction().y;
  return {Intersection{t, shared_from_this()}};
}

vector_t Plane::localNormalsAt(const point_t &objectPoint) const {
  return Vector(0, 1, 0);
};

bool Plane::operator==(const Shape &other) const {
  const Plane *s = dynamic_cast<const Plane *>(&other);
  return s && Shape::operator==(other);
}
