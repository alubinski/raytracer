#include "triangle.h"
#include "intersection.h"
#include "ray.h"
#include "tuple.h"
#include <cmath>

Triangle::Triangle(point_t p1, point_t p2, point_t p3)
    : p1_(p1), p2_(p2), p3_(p3) {
  e1_ = p2 - p1;
  e2_ = p3 - p1;
  normal_ = crossProduct(e2_, e1_).normalize();
}

std::vector<Intersection> Triangle::intersept(const Ray &ray) const {
  const auto dirCrossE2 = crossProduct(ray.direction(), e2_);
  const float det = dotProduct(e1_, dirCrossE2);
  if (epsilonEqual(det, 0.f)) {
    return {};
  }

  const float f = 1.f / det;

  const auto p1ToOrigin = ray.origin() - p1_;
  const float u = f * dotProduct(p1ToOrigin, dirCrossE2);
  if (u < epsilon || u > 1.f - epsilon) {
    return {};
  }

  const auto originCrossE1 = crossProduct(p1ToOrigin, e1_);
  const float v = f * dotProduct(ray.direction(), originCrossE1);
  if (v < epsilon || u + v > 1.f - epsilon) {
    return {};
  }

  const float t = f * dotProduct(e2(), originCrossE1);

  return {makeInterseption(t, u, v)};
}

vector_t Triangle::localNormalsAt(const point_t &objectPoint,
                                  const Intersection &hit) const {
  return normal_;
}
