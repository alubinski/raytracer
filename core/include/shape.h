#pragma once

#include "material.h"
#include "matrix.h"
#include "tuple.h"
#include <optional>

class Ray;
class Intersection;

#include <memory>
class Shape : public std::enable_shared_from_this<Shape> {
public:
  using Ptr = std::shared_ptr<Shape>;
  using ConstPtr = std::shared_ptr<const Shape>;
  using UPtr = std::unique_ptr<Shape>;
  using ConstUPtr = std::unique_ptr<const Shape>;

  Shape() : transformation_(Mat44::identity()), material_(Material()) {}

  virtual std::optional<std::pair<Intersection, Intersection>>
  intersept(const Ray &ray) const = 0;
  virtual vector_t normalsAt(const point_t &p) const = 0;
  Mat44 transformation() const { return transformation_; }
  void setTransformation(const Mat44 &m) { transformation_ = m; }

  const Material &material() const { return material_; }

  Material &material() { return material_; }

  void setMaterial(const Material &material) { material_ = material; }

  virtual bool operator==(const Shape &other) const = 0;
  // {
  //   return transformation() == other.transformation() &&
  //          material() == other.material();
  // }

private:
  Mat44 transformation_;
  Material material_;
};
