#pragma once

#include "intersection.h"
#include "ray.h"
#include "shape.h"
#include "tuple.h"
#include <cmath>
#include <vector>

class Plane : public Shape {
public:
  Plane() : Shape() {
    auto &b = bounds();
    b.min() = Point(-INFINITY, 0.f, -INFINITY);
    b.max() = Point(INFINITY, 0.f, INFINITY);
  }

  std::vector<Intersection> intersept(const Ray &ray) const override;

  vector_t localNormalsAt(const point_t &objectPoint,
                          const Intersection &hit) const override;

  bool operator==(const Shape &other) const override;
};
