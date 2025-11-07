#include "ray.h"
#include "computation.h"
#include "intersection.h"
#include "tuple.h"
#include "types.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <optional>
#include <ranges>
#include <vector>

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

std::vector<Intersection> Ray::intersept(const ShapePtr &sphere) const {
  const Ray r = (*this) * sphere->transformation().inverse();
  return sphere->intersept(r);
}

std::vector<Intersection> Ray::intersept(const World &world) const {
  auto all = world.objects() | std::views::transform([this](const auto &obj) {
               return intersept(obj);
             }) |
             std::views::join;

  std::vector<Intersection> xs;

  std::ranges::copy(all, std::back_inserter(xs));

  std::sort(xs.begin(), xs.end());
  return xs;
}

Ray Ray::operator*(const Mat44 &m) const {
  return Ray{m * origin_, m * direction_};
}

ComputationData
Ray::precompute(const Intersection &intersection,
                std::optional<std::vector<Intersection>> xs) const {
  ComputationData data{};

  data.t = intersection.t();
  data.object = intersection.object();

  data.point = position(data.t);
  data.eyeV = -direction();
  data.normalV = data.object->normalsAt(data.point);
  if (dotProduct(data.eyeV, data.normalV) < 0.f) {
    data.inside = true;
    data.normalV = -data.normalV;
  } else {
    data.inside = false;
  }
  data.overPoint = data.point + data.normalV * epsilon;
  data.underPoint = data.point - data.normalV * epsilon;
  data.reflectiveV = direction().reflect(data.normalV);

  std::vector<ShapeConstPtr> containers;
  for (const auto &curr : xs.value_or(std::vector<Intersection>{})) {
    if (curr == intersection) {
      data.n1 = (containers.empty())
                    ? 1.f
                    : containers.back()->material().reflectiveIndex();
    }

    auto it = std::find(containers.begin(), containers.end(), curr.object());
    if (it != containers.end()) {
      containers.erase(it);
    } else {
      containers.push_back(curr.object());
    }

    if (curr == intersection) {
      data.n2 = (containers.empty())
                    ? 1.f
                    : containers.back()->material().reflectiveIndex();
    }
  }

  return data;
}
