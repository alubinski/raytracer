#include "cube.h"
#include "intersection.h"
#include "ray.h"
#include "tuple.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <utility>

std::vector<Intersection> Cube::intersept(const Ray &ray) const {
  const auto [xtimin, xtmax] = checkAxis(ray.origin().x, ray.direction().x);
  const auto [ytimin, ytmax] = checkAxis(ray.origin().y, ray.direction().y);
  const auto [ztimin, ztmax] = checkAxis(ray.origin().z, ray.direction().z);

  const auto tmin = std::max({xtimin, ytimin, ztimin});
  const auto tmax = std::min({xtmax, ytmax, ztmax});

  if (tmin > tmax) {
    return {};
  }

  return {Intersection(tmin, shared_from_this()),
          Intersection(tmax, shared_from_this())};
}

std::pair<float, float> Cube::checkAxis(const float origin,
                                        const float direction) const {
  const float tminNumerator = -1.f - origin;
  const float tmaxNumerator = 1.f - origin;

  float tmin, tmax;
  if (std::abs(direction) >= epsilon) {
    tmin = tminNumerator / direction;
    tmax = tmaxNumerator / direction;
  } else {
    tmin = tminNumerator * INFINITY;
    tmax = tmaxNumerator * INFINITY;
  }

  if (tmin > tmax) {
    std::swap(tmin, tmax);
  }

  return {tmin, tmax};
}

vector_t Cube::localNormalsAt(const point_t &objectPoint) const {
  const float maxc = std::max({std::abs(objectPoint.x), std::abs(objectPoint.y),
                               std::abs(objectPoint.z)});

  if (maxc == std::abs(objectPoint.x)) {
    return Vector(objectPoint.x, 0.f, 0.f);
  }

  if (maxc == std::abs(objectPoint.y)) {
    return Vector(0.f, objectPoint.y, 0.f);
  }

  return Vector(0.f, 0.f, objectPoint.z);
}
