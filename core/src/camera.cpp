#include "camera.h"
#include "ray.h"
#include "tuple.h"
#include <cmath>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

Camera::Camera(uint32_t hSize, uint32_t vSize, float fieldofView)
    : hSize_(hSize), vSize_(vSize), fieldOfView_(fieldofView) {
  const float halfView = std::tanf(fieldofView / 2);
  const float aspect = (float)hSize / vSize;
  if (aspect > 1.f) {
    halfWidth_ = halfView;
    halfHeight_ = halfView / aspect;
  } else {
    halfWidth_ = halfView * aspect;
    halfHeight_ = halfView;
  }
  pixelSize_ = (halfWidth_ * 2) / hSize;
}

Ray Camera::rayForPixel(uint32_t x, uint32_t y) const {
  // the offset from the edge of the canvas to the pixel's center
  const float xoffset = (float(x) + 0.5f) * pixelSize_;
  const float yoffset = (float(y) + 0.5f) * pixelSize_;

  // the untransformed coordinates of the pixel in world space.
  // (remember that the camera looks toward -z, so +x is to the *left*.)
  const float worldX = halfWidth_ - xoffset;
  const float worldY = halfHeight_ - yoffset;

  //  using the camera matrix, transform the canvas point and the origin,
  // and then compute the ray's direction vector.
  // (remember that th e canvas is at z=-1)
  auto pixel = transform().inverse() * Point(worldX, worldY, -1);
  pixel.w = 1;
  auto origin = transform().inverse() * Point(0, 0, 0);
  origin.w = 1;

  const auto direction = (pixel - origin).normalize();

  return Ray(origin, direction);
}

void renderRow(const Camera &camera, const World &world, Canvas &canvas,
               size_t rowBegin, size_t rowEnd, size_t cols) {
  for (size_t y = rowBegin; y < rowEnd; y++) {
    for (size_t x = 0; x < cols; x++) {
      Ray const r = camera.rayForPixel(x, y);
      canvas.writePixel(x, y, world.colorAt(r));
    }
  }
}

constexpr size_t RENDER_TASKS = 8;
Canvas Camera::render(const World &world) const {
  Canvas image(hSize_, vSize_);
  // Canvas image(vSize_, hSize_);

  std::vector<std::thread> tp;
  size_t rowsPerThread = vSize_ / RENDER_TASKS;

  for (size_t y = 0; y < (RENDER_TASKS - 1); ++y) {
    const size_t begin = y * rowsPerThread;
    const uint32_t end = begin + rowsPerThread;
    tp.push_back(std::thread(renderRow, std::ref(*this), std::ref(world),
                             std::ref(image), begin, end, hSize_));
  }

  const size_t begin = (RENDER_TASKS - 1) * rowsPerThread;
  renderRow(*this, world, image, begin, vSize_, hsize());

  for (auto &t : tp) {
    t.join();
  }

  // for (uint32_t y = 0; y < vSize_; ++y) {
  //   for (uint32_t x = 0; x < hSize_; ++x) {
  //     const auto ray = rayForPixel(x, y);
  //     const auto color = world.colorAt(ray);
  //     image.writePixel(x, y, color);
  //   }
  // }
  return image;
}
