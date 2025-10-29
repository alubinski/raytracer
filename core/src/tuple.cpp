#include "tuple.h"
#include "math.h"
#include <cmath>

bool Tuple::isPoint() const { return w == 1; }

bool Tuple::isVector() const { return w == 0; }

float Tuple::magnitude() const {
  return std::sqrt(x * x + y * y + z * z + w * w);
}

Tuple Tuple::normalize() const {
  return Tuple(x / magnitude(), y / magnitude(), z / magnitude(),
               w / magnitude());
}

Tuple Vector(float x, float y, float z) { return Tuple(x, y, z, 0); }

Tuple Point(float x, float y, float z) { return Tuple(x, y, z, 1); }

bool operator==(const Tuple &lhs, const Tuple &rhs) {
  return epsilonEqual(lhs.x, rhs.x) && epsilonEqual(lhs.y, rhs.y) &&
         epsilonEqual(lhs.z, rhs.z) && epsilonEqual(lhs.w, rhs.w);
}

Tuple operator+(const Tuple &lhs, const Tuple &rhs) {
  return Tuple{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w};
}

Tuple operator-(const Tuple &lhs, const Tuple &rhs) {
  return Tuple{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w};
}

Tuple operator*(const Tuple &lhs, const float &rhs) {
  return Tuple{lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs};
}

Tuple operator*(const float &lhs, const Tuple &rhs) { return rhs * lhs; }

Tuple operator/(const Tuple &lhs, const float &rhs) {
  return Tuple{lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs};
}
Tuple operator-(const Tuple &value) {
  return Tuple{-value.x, -value.y, -value.z, -value.w};
}

float dotProduct(const Tuple &lhs, const Tuple &rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

Tuple crossProduct(const Tuple &lhs, const Tuple &rhs) {
  return Vector(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z,
                lhs.x * rhs.y - lhs.y * rhs.x);
}
