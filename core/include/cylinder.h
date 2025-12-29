#pragma once

#include "shape.h"
#include <vector>

class Cylinder : public Shape {
public:
  Cylinder();

  Cylinder(float min, float max, bool closed);

  std::vector<Intersection> intersept(const Ray &ray) const override;

  vector_t localNormalsAt(const point_t &objectPoint,
                          const Intersection &hit) const override;

  const float &minimum() const { return minimum_; }

  const float &maximum() const { return maximum_; }

  const bool &closed() const { return closed_; }

  float &minimum() { return minimum_; }

  float &maximum() { return maximum_; }

  bool &closed() { return closed_; }

protected:
  float minimum_;
  float maximum_;
  bool closed_;

  bool checkCap(const Ray &ray, float t, float radius) const;

  void inteseptCaps(const Ray &ray, std::vector<Intersection> &xs) const;

  void updateBounds() override;

private:
  virtual float a(const Ray &ray) const;
  virtual float b(const Ray &ray) const;
  virtual float c(const Ray &ray) const;
  virtual float radiusAt(const float y) const { return 1.f; };
};
