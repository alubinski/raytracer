#pragma once

#include "color.h"
#include "tuple.h"
#include "types.h"
#include <cmath>

struct ComputationData {
  float t;
  ShapeConstPtr object;
  point_t point;
  vector_t eyeV;
  vector_t normalV;
  bool inside;
  point_t overPoint;
  point_t underPoint;
  vector_t reflectiveV;
  float n1;
  float n2;
};

inline std::ostream &operator<<(std::ostream &os, const ComputationData &c) {
  os << "ComputationData {\n"
     << "  t: " << c.t << ",\n"
     << "  object: "
     << (c.object ? "Shape@" + std::to_string(
                                   reinterpret_cast<uintptr_t>(c.object.get()))
                  : "null")
     << ",\n"
     << "  point: " << c.point << ",\n"
     << "  eyeV: " << c.eyeV << ",\n"
     << "  normalV: " << c.normalV << ",\n"
     << "  inside: " << std::boolalpha << c.inside << "\n"
     << "}";
  return os;
}

inline float schlick(const ComputationData &comps) {
  // find the cosine of the angle between the eye and normal vectors
  float cos = dotProduct(comps.eyeV, comps.normalV);

  // total internal reflection can only occur if n1 > n2
  if (comps.n1 > comps.n2) {
    const float n = comps.n1 / comps.n2;
    const float sin2_t = n * n * (1.f - cos * cos);
    if (sin2_t > 1.f) {
      return 1.f;
    }

    // compute cosine of theta_t using trig identity
    const float cos_t = std::sqrt(1.f - sin2_t);

    // when n1 > n2, use cos(theta_t) instead
    cos = cos_t;
  }

  const float r0 = ((comps.n1 - comps.n2) / (comps.n1 + comps.n2)) *
                   ((comps.n1 - comps.n2) / (comps.n1 + comps.n2));
  return r0 + (1.f - r0) * std::pow(1.f - cos, 5);
}
