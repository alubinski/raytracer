#pragma once
#include "matrix.h"
#include "tuple.h"

class Ray;

class Bounds {
public:
  Bounds();
  Bounds(point_t min, point_t max);

  const point_t &min() const { return min_; }
  point_t &min() { return min_; }

  const point_t &max() const { return max_; }
  point_t &max() { return max_; }

  void merge(const Bounds &other);

  bool intersects(const Ray &ray) const;

private:
  std::pair<float, float> checkAxis(const float origin, const float direction,
                                    const float min, const float max) const;

  point_t min_;
  point_t max_;
};

Bounds operator*(const Bounds &b, const Mat44 &mat);
