#pragma once

#include "intersection.h"
#include "shape.h"
#include "tuple.h"
#include <optional>
#include <utility>

class Ray;

class Sphere : public Shape {
public:
  Sphere() : Shape() {}

  std::optional<std::pair<Intersection, Intersection>>
  intersept(const Ray &ray) const;
  vector_t normalsAt(const point_t &worldPoint) const;

  bool operator==(const Shape &other) const override;

private:
};
