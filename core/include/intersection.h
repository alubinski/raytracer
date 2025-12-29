#pragma once

#include "types.h"
#include <algorithm>
#include <cmath>
#include <optional>
#include <utility>
#include <vector>

class Intersection {
public:
  Intersection() = default;
  Intersection(float t, const ShapeConstPtr &s)
      : t_(t), shape_(s), u_(std::numeric_limits<float>::quiet_NaN()),
        v_(std::numeric_limits<float>::quiet_NaN()) {}
  Intersection(float t, const ShapeConstPtr &s, float u, float v)
      : t_(t), shape_(s), u_(u), v_(v) {}

  float t() const { return t_; }
  ShapeConstPtr object() const { return shape_; }
  float u() const { return u_; }
  float v() const { return v_; }

  bool operator==(const Intersection &other) const {
    return t_ == other.t_ && shape_ == other.shape_;
  }

  bool operator<(const Intersection &other) const { return t_ < other.t_; }

private:
  float t_;
  ShapeConstPtr shape_;
  float u_;
  float v_;
};

inline std::vector<Intersection> to_vector(const Intersection &s) {
  return {s};
}

inline std::vector<Intersection>
to_vector(const std::pair<Intersection, Intersection> &p) {
  return {p.first, p.second};
}

inline std::vector<Intersection>
to_vector(const std::optional<std::pair<Intersection, Intersection>> &p) {
  if (p) {
    return {p->first, p->second};
  }
  return {};
}

template <typename... Args>
std::vector<Intersection> intersections(const Args &...args) {
  std::vector<Intersection> xs;
  (([&] {
     for (auto &i : to_vector(args))
       xs.push_back(i);
   }()),
   ...);
  std::sort(xs.begin(), xs.end());
  return xs;
}

std::optional<const Intersection> hit(const std::vector<Intersection> &xs);
