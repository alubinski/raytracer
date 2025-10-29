#pragma once

struct Tuple {
  Tuple(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

  bool isVector() const;
  bool isPoint() const;

  float magnitude() const;
  Tuple normalize() const;

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
