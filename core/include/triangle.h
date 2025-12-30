#pragma once

#include "intersection.h"
#include "shape.h"
#include "tuple.h"

class Triangle : public Shape {
public:
  Triangle(point_t p1, point_t p2, point_t p3);

  virtual std::vector<Intersection> intersept(const Ray &ray) const override;

  virtual vector_t localNormalsAt(const point_t &objectPoint,
                                  const Intersection &hit) const override;

  const point_t p1() const { return p1_; }
  const point_t p2() const { return p2_; }
  const point_t p3() const { return p3_; }

  const vector_t e1() const { return e1_; }
  const vector_t e2() const { return e2_; }
  const vector_t normal() const { return normal_; }

protected:
  point_t p1_;
  point_t p2_;
  point_t p3_;

  vector_t e1_;
  vector_t e2_;
  vector_t normal_;

  void updateBounds() override;

private:
  virtual Intersection makeInterseption(float t, float u, float v) const {
    return Intersection(t, shared_from_this());
  }
};
