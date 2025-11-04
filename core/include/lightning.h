#pragma once

#include "color.h"
#include "material.h"
#include "tuple.h"

class PointLight {
public:
  PointLight(const point_t position, const Color intensity);
  ~PointLight() = default;

  PointLight(const PointLight &other) = default;

  PointLight &operator=(const PointLight &other) = default;

  point_t position() const { return position_; }
  Color intensity() const { return intensity_; }

  bool operator==(const PointLight &other) const;

private:
  point_t position_;
  Color intensity_;
};

Color lightining(const Material &material, const PointLight &light,
                 const point_t &position, const vector_t &eyeVec,
                 const vector_t &normalVec);
