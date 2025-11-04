#pragma once
#include "tuple.h"

struct Color : public Tuple {
public:
  Color() : Tuple(0, 0, 0, 0) {}
  Color(float red, float green, float blue) : Tuple(red, green, blue, 0) {}
  explicit Color(const Tuple &other)
      : Tuple(other.x, other.y, other.z, other.w) {}

  Color(const Color &) = default;
  Color(Color &&) noexcept = default;
  Color &operator=(const Color &) = default;
  Color &operator=(Color &&) noexcept = default;

  float &r() noexcept { return x; }
  float &g() noexcept { return y; }
  float &b() noexcept { return z; }

  const float &r() const noexcept { return x; }
  const float &g() const noexcept { return y; }
  const float &b() const noexcept { return z; }

  // Hadamard product example
  friend Color operator*(const Color &lhs, const Color &rhs) noexcept {
    return Color(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
  }

  friend Color operator+(const Color &lhs, const Color &rhs) noexcept {
    return Color(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
  }

  friend Color operator*(const Color &lhs, const float &rhs) noexcept {
    return Color(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
  }

  static const Color &white() {
    static Color c(1.f, 1.f, 1.f);
    return c;
  }
  static const Color &black() {
    static Color c(0.f, 0.f, 0.f);
    return c;
  }
  static const Color &red() {
    static Color c(1.f, 0.f, 0.f);
    return c;
  }
  static const Color &green() {
    static Color c(0.f, 1.f, 0.f);
    return c;
  }
  static const Color &blue() {
    static Color c(0.f, 0.f, 1.f);
    return c;
  }
  static const Color &yellow() {
    static Color c(1.f, 1.f, 0.f);
    return c;
  }

private:
  using Tuple::w;
};
