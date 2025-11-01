#pragma once

#include "types.h"
#include <algorithm>
#include <optional>
#include <utility>
#include <vector>

class Intersection {
public:
  Intersection(float t, const ShapeConstPtr &s) : t_(t), shape_(s) {}

  float t() const { return t_; }
  ShapeConstPtr object() const { return shape_; }

  bool operator==(const Intersection &other) const {
    return t_ == other.t_ && shape_ == other.shape_;
  }

  bool operator<(const Intersection &other) const { return t_ < other.t_; }

private:
  float t_;
  ShapeConstPtr shape_;
};

inline std::vector<Intersection> to_vector(const Intersection &s) {
  return {s};
}

inline std::vector<Intersection>
to_vector(const std::pair<Intersection, Intersection> &p) {
  return {p.first, p.second};
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

std::optional<std::reference_wrapper<const Intersection>>
hit(const std::vector<Intersection> &xs);
