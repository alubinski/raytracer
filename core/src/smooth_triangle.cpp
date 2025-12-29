#include "smooth_triangle.h"

SmoothTriangle ::SmoothTriangle(point_t p1, point_t p2, point_t p3, vector_t n1,
                                vector_t n2, vector_t n3)
    : Triangle(p1, p2, p3), n1_(n1), n2_(n2), n3_(n3) {}

vector_t SmoothTriangle::localNormalsAt(const point_t &objectPoint,
                                        const Intersection &hit) const {
  return n2() * hit.u() + n3() * hit.v() + n1() * (1 - hit.u() - hit.v());
}
