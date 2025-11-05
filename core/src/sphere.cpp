#include "sphere.h"
#include "intersection.h"
#include "ray.h"
#include "tuple.h"
#include <cmath>
#include <optional>
#include <utility>

std::optional<std::pair<Intersection, Intersection>>
Sphere::intersept(const Ray &ray) const {
  const auto sphereToRay = ray.origin() - Point(0, 0, 0);

  const auto a = dotProduct(ray.direction(), ray.direction());
  const auto b = 2 * dotProduct(ray.direction(), sphereToRay);
  const auto c = dotProduct(sphereToRay, sphereToRay) - 1.f;

  const auto discriminant = b * b - 4 * a * c;
  if (discriminant < 0) {
    return std::nullopt;
  }

  const auto t1 = (-b - std::sqrt(discriminant)) / (2 * a);
  const auto t2 = (-b + std::sqrt(discriminant)) / (2 * a);
  return std::make_pair(Intersection(t1, shared_from_this()),
                        Intersection(t2, shared_from_this()));
}

vector_t Sphere::localNormalsAt(const point_t &objectPoint) const {
  return objectPoint - Point(0, 0, 0);
};

bool Sphere::operator==(const Shape &other) const {
  const Sphere *s = dynamic_cast<const Sphere *>(&other);
  return s && Shape::operator==(other);
}
