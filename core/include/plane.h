#pragma once

#include "intersection.h"
#include "ray.h"
#include "shape.h"
#include "tuple.h"
#include <vector>

class Plane : public Shape {
public:
  Plane() : Shape() {}

  std::vector<Intersection> intersept(const Ray &ray) const override;

  vector_t localNormalsAt(const point_t &objectPoint) const override;

  bool operator==(const Shape &other) const override;
};
