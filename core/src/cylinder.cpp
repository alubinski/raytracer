#include "cylinder.h"
#include "intersection.h"
#include "math.h"
#include "ray.h"
#include "tuple.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

Cylinder::Cylinder()
    : minimum_(-std::numeric_limits<float>::infinity()),
      maximum_(std::numeric_limits<float>::infinity()), closed_(false) {}

Cylinder::Cylinder(float min, float max, bool closed)
    : minimum_(min), maximum_(max), closed_(closed) {}

std::vector<Intersection> Cylinder::intersept(const Ray &ray) const {

  const float a = this->a(ray);

  std::vector<Intersection> xs{};

  // ray is paraller to y axis
  if (epsilonEqual(a, 0.f)) {
    inteseptCaps(ray, xs);
    return xs;
  }

  const float b = this->b(ray);

  const float c = this->c(ray);

  float disc = b * b - 4 * a * c;
  if (epsilonEqual(disc, 0.f)) {
    disc = 0;
  }

  if (disc < 0.f) {
    inteseptCaps(ray, xs);
    return xs;
  }

  float t1 = (-b - std::sqrt(disc)) / (2 * a);
  float t2 = (-b + std::sqrt(disc)) / (2 * a);

  if (t1 > t2)
    std::swap(t1, t2);

  const float y0 = ray.origin().y + t1 * ray.direction().y;
  if (minimum_ < y0 && y0 < maximum_) {

    xs.push_back(Intersection{t1, shared_from_this()});
  }

  const float y1 = ray.origin().y + t2 * ray.direction().y;
  if (minimum_ < y1 && y1 < maximum_) {
    xs.push_back(Intersection{t2, shared_from_this()});
  }

  inteseptCaps(ray, xs);
  return xs;
}

vector_t Cylinder::localNormalsAt(const point_t &objectPoint) const {
  auto dist = objectPoint.x * objectPoint.x + objectPoint.z * objectPoint.z;

  if (dist < 1.0f && epsilonEqual(objectPoint.y, maximum_)) {
    return Vector(0, 1, 0);
  }
  if (dist < 1.0f && epsilonEqual(objectPoint.y, minimum_)) {
    return Vector(0, -1, 0);
  }

  return Vector(objectPoint.x, 0, objectPoint.z);
}

bool Cylinder::checkCap(const Ray &ray, float t, float radius) const {
  float x = ray.origin().x + t * ray.direction().x;
  float z = ray.origin().z + t * ray.direction().z;

  return x * x + z * z <= radius + epsilon;
}

void Cylinder::inteseptCaps(const Ray &ray,
                            std::vector<Intersection> &xs) const {
  if (!closed_ || epsilonEqual(ray.direction().y, 0.f)) {
    return;
  }

  auto t = (minimum_ - ray.origin().y) / ray.direction().y;
  if (checkCap(ray, t, radiusAt(minimum_))) {
    xs.push_back({t, shared_from_this()});
  }

  t = (maximum_ - ray.origin().y) / ray.direction().y;
  if (checkCap(ray, t, radiusAt(maximum_))) {
    xs.push_back({t, shared_from_this()});
  }
}

float Cylinder::a(const Ray &ray) const {
  const auto &dir = ray.direction();
  return dir.x * dir.x + dir.z * dir.z;
}

float Cylinder::b(const Ray &ray) const {
  const auto &dir = ray.direction();
  const auto &orig = ray.origin();

  return 2.f * (dir.x * orig.x + dir.z * orig.z);
}

float Cylinder::c(const Ray &ray) const {
  const auto &orig = ray.origin();

  return orig.x * orig.x + orig.z * orig.z - 1.f;
}
