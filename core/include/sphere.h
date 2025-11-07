#pragma once

#include "intersection.h"
#include "shape.h"
#include "tuple.h"
#include <vector>

class Ray;

class Sphere : public Shape {
public:
  Sphere() : Shape() {}

  static Sphere GlassSphere() {
    Sphere s;
    s.material().setTransparency(1.f);
    s.material().setReflectiveIndex(1.5f);
    return s;
  }

  std::vector<Intersection> intersept(const Ray &ray) const override;

  vector_t localNormalsAt(const point_t &objectPoint) const override;

  bool operator==(const Shape &other) const override;

private:
};
