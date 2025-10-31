#pragma once

#include "matrix.h"

struct Tuple {
  Tuple(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

  bool isVector() const;
  bool isPoint() const;

  float magnitude() const;
  Tuple normalize() const;

  Matrix<4, 1> toMatrix() const { return Matrix<4, 1>{{x}, {y}, {z}, {w}}; }
  // Matrix<4, 1> toMatrix() const { return Matrix<4, 1>{x, y, z, w}; }

  static Tuple fromMatrix(const Matrix<4, 1> &m) {
    return Tuple{m(0, 0), m(1, 0), m(2, 0), m(3, 0)};
  }

  friend std::ostream &operator<<(std::ostream &os, const Tuple &t) {
    return os << "{" << t.x << ", " << t.y << ", " << t.z << ", " << t.w << "}";
  }

  float x, y, z, w;
};

using point_t = Tuple;
using vector_t = Tuple;

Tuple Vector(float x, float y, float z);
Tuple Point(float x, float y, float z);

bool operator==(const Tuple &lhs, const Tuple &rhs);
Tuple operator+(const Tuple &lhs, const Tuple &rhs);
Tuple operator-(const Tuple &lhs, const Tuple &rhs);
Tuple operator*(const Tuple &lhs, const float &rhs);
Tuple operator*(const float &lhs, const Tuple &rhs);
Tuple operator/(const Tuple &lhs, const float &rhs);
Tuple operator-(const Tuple &other);

float dotProduct(const Tuple &lhs, const Tuple &rhs);
Tuple crossProduct(const Tuple &lhs, const Tuple &rhs);
