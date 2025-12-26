#pragma once

#include "cylinder.h"

class Cone : public Cylinder {
public:
  Cone() : Cylinder() {};
  Cone(float min, float max, bool closed) : Cylinder(min, max, closed) {}

  std::vector<Intersection> intersept(const Ray &ray) const override;
  vector_t localNormalsAt(const point_t &objectPoint) const override;

private:
  float a(const Ray &ray) const override;
  float b(const Ray &ray) const override;
  float c(const Ray &ray) const override;
  float radiusAt(const float y) const override;
};
