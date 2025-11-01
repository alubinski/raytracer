#pragma once

#include "matrix.h"
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

  Shape() : transformation_(Mat44::identity()) {}

  virtual std::optional<std::pair<Intersection, Intersection>>
  intersept(const Ray &ray) const = 0;
  Mat44 transformation() const { return transformation_; }
  void setTransformation(const Mat44 &m) { transformation_ = m; }

private:
  Mat44 transformation_;
};
