#pragma once

#include "cylinder.h"
#include <cmath>

class Cone : public Cylinder {
public:
  Cone() : Cone(-INFINITY, INFINITY, false) {};
  Cone(float min, float max, bool closed) : Cylinder(min, max, closed) {
    updateBounds();
  }

  std::vector<Intersection> intersept(const Ray &ray) const override;
  vector_t localNormalsAt(const point_t &objectPoint,
                          const Intersection &hit) const override;

protected:
  void updateBounds() override;

private:
  float a(const Ray &ray) const override;
  float b(const Ray &ray) const override;
  float c(const Ray &ray) const override;
  float radiusAt(const float y) const override;
};
