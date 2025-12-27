#include "bounds.h"
#include "ray.h"
#include "tuple.h"
#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

Bounds::Bounds() : Bounds(Point(-1, -1, -1), Point(1, 1, 1)) {}

Bounds::Bounds(point_t min, point_t max) : min_{min}, max_{max} {}

Bounds operator*(const Bounds &b, const Mat44 &m) {
  std::vector<point_t> verticles{m * Point(b.min().x, b.min().y, b.min().z),
                                 m * Point(b.min().x, b.max().y, b.min().z),
                                 m * Point(b.max().x, b.min().y, b.min().z),
                                 m * Point(b.max().x, b.max().y, b.min().z),
                                 m * Point(b.min().x, b.min().y, b.max().z),
                                 m * Point(b.min().x, b.max().y, b.max().z),
                                 m * Point(b.max().x, b.min().y, b.max().z),
                                 m * Point(b.max().x, b.max().y, b.max().z)};

  point_t max = Point(-INFINITY, -INFINITY, -INFINITY);
  point_t min = Point(INFINITY, INFINITY, INFINITY);

  for (const auto &v : verticles) {
    min.x = std::min(min.x, v.x);
    min.y = std::min(min.y, v.y);
    min.z = std::min(min.z, v.z);

    max.x = std::max(max.x, v.x);
    max.y = std::max(max.y, v.y);
    max.z = std::max(max.z, v.z);
  }
  return {min, max};
}

void Bounds::merge(const Bounds &other) {
  min_ = Point(std::min(min_.x, other.min().x), std::min(min_.y, other.min().y),
               std::min(min_.z, other.min().z));

  max_ = Point(std::max(max_.x, other.max().x), std::max(max_.y, other.max().y),
               std::max(max_.z, other.max().z));
}

std::pair<float, float> Bounds::checkAxis(const float origin,
                                          const float direction,
                                          const float min,
                                          const float max) const {

  const float oneOverDir = 1.f / direction;
  float tMin = (min - origin) * oneOverDir;
  float tMax = (max - origin) * oneOverDir;
  if (tMin > tMax) {
    std::swap(tMin, tMax);
  }
  return {tMin, tMax};
}

bool Bounds::intersects(const Ray &ray) const {
  const auto [xtimin, xtmax] =
      checkAxis(ray.origin().x, ray.direction().x, min().x, max().x);
  const auto [ytimin, ytmax] =
      checkAxis(ray.origin().y, ray.direction().y, min().y, max().y);
  const auto [ztimin, ztmax] =
      checkAxis(ray.origin().z, ray.direction().z, min().z, max().z);

  const auto tmin = std::max({xtimin, ytimin, ztimin});
  const auto tmax = std::min({xtmax, ytmax, ztmax});
  return tmin <= tmax;
}
