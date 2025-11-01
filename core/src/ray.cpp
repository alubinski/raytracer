#include "ray.h"
#include "intersection.h"
#include "tuple.h"
#include "types.h"
#include <cassert>

Ray::Ray(const point_t &origin, const vector_t &direction)
    : origin_(origin), direction_(direction) {
  if (!origin.isPoint())
    throw std::invalid_argument("Ray origin must be a point (w == 1)");
  if (!direction.isVector())
    throw std::invalid_argument("Ray direction must be a vector (w =0)");
}

const point_t &Ray::origin() const { return origin_; }

const vector_t &Ray::direction() const { return direction_; }

point_t Ray::position(float t) const { return origin_ + direction_ * t; }

std::optional<std::pair<Intersection, Intersection>>
Ray::intersept(const ShapePtr &sphere) const {
  const Ray r = (*this) * sphere->transformation().inverse();
  return sphere->intersept(r);
}

Ray Ray::operator*(const Mat44 &m) const {
  return Ray{m * origin_, m * direction_};
}
