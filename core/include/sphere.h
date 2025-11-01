#pragma once

#include "intersection.h"
#include "shape.h"
#include "tuple.h"
#include "types.h"
#include <optional>
#include <utility>

class Ray;

class Sphere : public Shape {
public:
  Sphere() : Shape() {}

  std::optional<std::pair<Intersection, Intersection>>
  intersept(const Ray &ray) const;

private:
};
