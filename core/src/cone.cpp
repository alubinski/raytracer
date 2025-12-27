#include "cone.h"
#include "intersection.h"
#include "ray.h"
#include "tuple.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <vector>

float Cone::a(const Ray &ray) const {
  const auto &dir = ray.direction();
  return dir.x * dir.x - dir.y * dir.y + dir.z * dir.z;
}

float Cone::b(const Ray &ray) const {
  const auto &dir = ray.direction();
  const auto &orig = ray.origin();
  return 2.f * (orig.x * dir.x - orig.y * dir.y + orig.z * dir.z);
}

float Cone::c(const Ray &ray) const {
  const auto &orig = ray.origin();
  return orig.x * orig.x - orig.y * orig.y + orig.z * orig.z;
}

std::vector<Intersection> Cone::intersept(const Ray &ray) const {
  if (epsilonEqual(this->a(ray), 0.f)) {
    std::vector<Intersection> xs;
    inteseptCaps(ray, xs);
    const float t = -c(ray) / (2 * b(ray));
    const float y = ray.origin().y + t * ray.direction().y;
    if (minimum_ < y && y < maximum_) {
      xs.push_back(Intersection{t, shared_from_this()});
      return xs;
    }
  }
  return Cylinder::intersept(ray);
}

float Cone::radiusAt(const float y) const { return std::abs(y); }

vector_t Cone::localNormalsAt(const point_t &objectPoint) const {
  auto dist = objectPoint.x * objectPoint.x + objectPoint.z * objectPoint.z;

  if (dist < 1.0f && epsilonEqual(objectPoint.y, maximum_)) {
    return Vector(0, 1, 0);
  }
  if (dist < 1.0f && epsilonEqual(objectPoint.y, minimum_)) {
    return Vector(0, -1, 0);
  }

  float y =
      std::sqrt(objectPoint.x * objectPoint.x + objectPoint.z * objectPoint.z);
  if (objectPoint.y > 0.f) {
    y = -y;
  }
  return Vector(objectPoint.x, y, objectPoint.z);
}

void Cone::updateBounds() {
  auto &b = bounds();
  const auto maxAbs = std::max(std::abs(minimum_), std::abs(maximum_));
  b.min() = Point(-maxAbs, minimum_, -maxAbs);
  b.max() = Point(maxAbs, maximum_, maxAbs);
}
