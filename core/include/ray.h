#pragma once

#include "intersection.h"
#include "sphere.h"
#include "tuple.h"
#include "types.h"
#include <optional>
#include <utility>

class Ray {
public:
  Ray(const point_t &origin, const vector_t &direction);

  const point_t &origin() const;

  const vector_t &direction() const;

  point_t position(float t) const;

  std::optional<std::pair<Intersection, Intersection>>
  intersept(const ShapePtr &shape) const;

  Ray operator*(const Mat44 &m) const;

private:
  point_t origin_;
  vector_t direction_;
};
