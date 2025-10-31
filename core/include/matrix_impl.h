#include "matrix.h"
#include "transformations.h"

template <>
inline Matrix<4, 4> Matrix<4, 4>::translate(float x, float y, float z) {
  return translation(x, y, z) * (*this);
}

template <> inline Matrix<4, 4> Matrix<4, 4>::scale(float x, float y, float z) {
  return scaling(x, y, z) * (*this);
}

template <> inline Matrix<4, 4> Matrix<4, 4>::rotateX(float radians) {
  return rotationX(radians) * (*this);
}

template <> inline Matrix<4, 4> Matrix<4, 4>::rotateY(float radians) {
  return rotationY(radians) * (*this);
}

template <> inline Matrix<4, 4> Matrix<4, 4>::rotateZ(float radians) {
  return rotationZ(radians) * (*this);
}

template <>
inline Matrix<4, 4> Matrix<4, 4>::shear(float xy, float xz, float yx, float yz,
                                        float zx, float zy) {
  return shearing(xy, xz, yx, yz, zx, zy) * (*this);
}
