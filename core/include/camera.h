#pragma once

#include "canvas.h"
#include "matrix.h"
#include "ray.h"
#include <cstdint>

class Camera {
public:
  Camera(uint32_t hSize, uint32_t vSize, float fieldofView);

  uint32_t hsize() const { return hSize_; }
  uint32_t vsize() const { return vSize_; }
  float fieldOfView() const { return fieldOfView_; }
  float pixelSize() const { return pixelSize_; }

  Mat44 &transform() { return transform_; }
  const Mat44 transform() const { return transform_; }

  void setTransorm(const Mat44 &m) { transform_ = m; }

  Ray rayForPixel(uint32_t x, uint32_t y) const;

  Canvas render(const World &world) const;

private:
  uint32_t hSize_;
  uint32_t vSize_;
  float fieldOfView_;
  Mat44 transform_ = Mat44::identity();
  float pixelSize_;
  float halfWidth_;
  float halfHeight_;
};
