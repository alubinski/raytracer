#pragma once

#include "material.h"
#include "matrix.h"
#include "tuple.h"
#include <vector>

class Ray;
class Intersection;
class Material;

#include <memory>
class Shape : public std::enable_shared_from_this<Shape> {
public:
  Shape() : transformation_(Mat44::identity()), material_(Material()) {}

  virtual std::vector<Intersection> intersept(const Ray &ray) const = 0;

  virtual vector_t localNormalsAt(const point_t &objectPoint) const = 0;

  vector_t normalsAt(const point_t &worldPoint) const {
    if (!worldPoint.isPoint()) {
      throw std::invalid_argument("Sphere::normalsAt() expects a point as "
                                  "input, but a non-point value was provided.");
    }
    const auto objectPoint = transformation().inverse() * worldPoint;
    const auto objectNormal = localNormalsAt(objectPoint);
    auto worldNormal = transformation().inverse().transpose() * objectNormal;
    // hack - noramlly we should take transformation.submatrix(3,3)
    worldNormal.w = 0.f;
    return worldNormal.normalize();
  }

  Mat44 transformation() const { return transformation_; }
  void setTransformation(const Mat44 &m) { transformation_ = m; }

  Mat44 &transformation() { return transformation_; }
  const Material &material() const { return material_; }

  Material &material() { return material_; }

  void setMaterial(const Material &material) { material_ = material; }

  virtual bool operator==(const Shape &other) const {
    return transformation() == other.transformation() &&
           material() == other.material();
  }

private:
  Mat44 transformation_;
  Material material_;
};
