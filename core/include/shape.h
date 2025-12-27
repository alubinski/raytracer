#pragma once

#include "material.h"
#include "matrix.h"
#include "tuple.h"
#include "types.h"
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
      throw std::invalid_argument("Shape::normalsAt() expects a point as "
                                  "input, but a non-point value was provided.");
    }
    const auto objectPoint = worldPointToObjectPoint(worldPoint);
    const auto objectNormal = localNormalsAt(objectPoint);
    return objectNormalToWorldNormal(objectNormal);
  }

  Mat44 transformation() const { return transformation_; }
  void setTransformation(const Mat44 &m) { transformation_ = m; }

  Mat44 &transformation() { return transformation_; }
  const Material &material() const { return material_; }

  Material &material() { return material_; }

  void setMaterial(const Material &material) { material_ = material; }

  bool castShadows() const { return castsShadows_; }

  void setCastsShadows(bool castShadows) { castsShadows_ = castShadows; }

  virtual bool operator==(const Shape &other) const {
    return transformation() == other.transformation() &&
           material() == other.material();
  }

  ShapePtr parent() { return parent_; }

  ShapeConstPtr parent() const { return parent_; }

  void setParent(ShapePtr parent) { parent_ = parent; }

  point_t worldPointToObjectPoint(point_t worldPoint) const {
    auto objectPoint =
        (parent_ ? parent_->worldPointToObjectPoint(worldPoint) : worldPoint);
    return transformation().inverse() * objectPoint;
  }

  vector_t objectNormalToWorldNormal(vector_t objectNormal) const {
    auto worldNormal = transformation().inverse().transpose() * objectNormal;
    worldNormal.w = 0;
    worldNormal = worldNormal.normalize();
    return parent_ ? parent_->objectNormalToWorldNormal(worldNormal)
                   : worldNormal;
  }

private:
  Mat44 transformation_;
  Material material_;
  bool castsShadows_{true};
  ShapePtr parent_;
};
