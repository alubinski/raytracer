#include "sphere.h"
#include "intersection.h"
#include "ray.h"
#include "tuple.h"
#include <cmath>
#include <optional>
#include <stdexcept>
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

vector_t Sphere::normalsAt(const point_t &worldPoint) const {
  if (!worldPoint.isPoint()) {
    throw std::invalid_argument("Sphere::normalsAt() expects a point as "
                                "input, but a non-point value was provided.");
  }
  const auto objectPoint = transformation().inverse() * worldPoint;
  const auto objectNormal = objectPoint - Point(0, 0, 0);
  auto worldNormal = transformation().inverse().transpose() * objectNormal;
  // hack - noramlly we should take transformation.submatrix(3,3)
  worldNormal.w = 0.f;
  return worldNormal.normalize();
}

bool Sphere::operator==(const Shape &other) const {
  const Sphere *s = dynamic_cast<const Sphere *>(&other);
  if (!s)
    return false; // different types
  return transformation() == s->transformation() && material() == s->material();
}
