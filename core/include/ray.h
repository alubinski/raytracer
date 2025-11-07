#pragma once

#include "computation.h"
#include "intersection.h"
#include "sphere.h"
#include "tuple.h"
#include "types.h"
#include "world.h"
#include <optional>
#include <utility>
#include <vector>

class Ray {
public:
  Ray(const point_t &origin, const vector_t &direction);

  const point_t &origin() const;

  const vector_t &direction() const;

  point_t position(float t) const;

  std::vector<Intersection> intersept(const ShapePtr &shape) const;
  std::vector<Intersection> intersept(const World &world) const;

  Ray operator*(const Mat44 &m) const;

  // ComputationData precompute(const Intersection &intersection) const;
  ComputationData
  precompute(const Intersection &intersection,
             std::optional<std::vector<Intersection>> xs = std::nullopt) const;

private:
  point_t origin_;
  vector_t direction_;
};

inline std::ostream &operator<<(std::ostream &os, const Ray &ray) {
  os << "Ray(origin=" << ray.origin() << ", direction=" << ray.direction()
     << ")";
  return os;
}
