#pragma once

#include "triangle.h"
#include "tuple.h"

class SmoothTriangle : public Triangle {
public:
  SmoothTriangle(point_t p1, point_t p2, point_t p3, vector_t n1, vector_t n2,
                 vector_t n3);

  virtual vector_t localNormalsAt(const point_t &objectPoint,
                                  const Intersection &hit) const override;

  vector_t n1() const { return n1_; }
  vector_t n2() const { return n2_; }
  vector_t n3() const { return n3_; }

private:
  vector_t n1_;
  vector_t n2_;
  vector_t n3_;

  virtual Intersection makeInterseption(float t, float u,
                                        float v) const override {
    return Intersection(t, shared_from_this(), u, v);
  }
};
