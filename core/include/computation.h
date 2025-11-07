#pragma once

#include "tuple.h"
#include "types.h"
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
