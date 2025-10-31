#pragma once
#include "matrix.h"
#include <cmath>

inline Matrix<4, 4> translation(float x, float y, float z) {
  return Matrix<4, 4>{{1.f, 0.f, 0.f, x},
                      {0.f, 1.f, 0.f, y},
                      {0.f, 0.f, 1.f, z},
                      {0.f, 0.f, 0.f, 1.f}};
}

inline Matrix<4, 4> scaling(float x, float y, float z) {
  return Matrix<4, 4>({{x, 0.f, 0.f, 0.f},
                       {0.f, y, 0.f, 0.f},
                       {0.f, 0.f, z, 0.f},
                       {0.f, 0.f, 0.f, 1.f}});
}

inline Matrix<4, 4> rotationX(float radians) {
  float const c = std::cos(radians);
  float const s = std::sin(radians);
  return Matrix<4, 4>({{1.f, 0.f, 0.f, 0.f},
                       {0.f, c, -s, 0.f},
                       {0.f, s, c, 0.f},
                       {0.f, 0.f, 0.f, 1.f}});
}

inline Matrix<4, 4> rotationY(float radians) {
  float const c = std::cos(radians);
  float const s = std::sin(radians);
  return Matrix<4, 4>({{c, 0.f, s, 0.f},
                       {0.f, 1.f, 0.f, 0.f},
                       {-s, 0.f, c, 0.f},
                       {0.f, 0.f, 0.f, 1.f}});
}

inline Matrix<4, 4> rotationZ(float radians) {
  float const c = std::cos(radians);
  float const s = std::sin(radians);
  return Matrix<4, 4>({{c, -s, 0.f, 0.f},
                       {s, c, 0.f, 0.f},
                       {0.f, 0.f, 1.f, 0.f},
                       {0.f, 0.f, 0.f, 1.f}});
}

inline Matrix<4, 4> shearing(float xy, float xz, float yx, float yz, float zx,
                             float zy) {
  return Matrix<4, 4>({{1.f, xy, xz, 0.f},
                       {yx, 1.f, yz, 0.f},
                       {zx, zy, 1.f, 0.f},
                       {0.f, 0.f, 0.f, 1.f}});
}
