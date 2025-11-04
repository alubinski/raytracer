#pragma once

#include "math.h"
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <ostream>
#include <stdexcept>

class Tuple;

template <uint8_t ROWS, uint8_t COLS> class Matrix {
public:
  Matrix() : data_{} {}
  Matrix(std::initializer_list<std::initializer_list<float>> data);

  template <uint8_t row, uint8_t col>
    requires(row < ROWS && col < COLS)
  float At() const {
    return data_[row][col];
  }

  float operator()(uint8_t row, uint8_t col) const {
    if (row >= ROWS || col >= COLS)
      throw std::out_of_range("Matrix indices out of range");
    return data_[row][col];
  }

  float &operator()(uint8_t row, uint8_t col) {
    if (row >= ROWS || col >= COLS)
      throw std::out_of_range("Matrix indices out of range");
    return data_[row][col];
  }

  bool operator==(const Matrix &other) const;
  bool operator!=(const Matrix &other) const {
    return !this->operator==(other);
  }

  template <uint8_t OTHER_ROWS, uint8_t OTHER_COLS>
    requires(COLS == OTHER_ROWS)
  Matrix<ROWS, OTHER_COLS>
  operator*(const Matrix<OTHER_ROWS, OTHER_COLS> &other) const {
    Matrix<ROWS, OTHER_COLS> result{};

    for (size_t i = 0; i < ROWS; ++i) {
      for (size_t j = 0; j < OTHER_COLS; ++j) {
        for (size_t k = 0; k < COLS; ++k) {
          result(i, j) += (*this)(i, k) * other(k, j);
        }
      }
    }
    return result;
  }

  template <typename TupleT>
    requires(COLS == 4)
  TupleT operator*(const TupleT &t) const {
    return TupleT::fromMatrix((*this) * t.toMatrix());
  }

  Matrix<COLS, ROWS> transpose() const {
    Matrix<COLS, ROWS> result{};
    for (size_t i = 0; i < ROWS; i++) {
      for (size_t j = 0; j < COLS; j++) {
        result(j, i) = data_[i][j];
      }
    }
    return result;
  }

  float determinant() const;

  Matrix<ROWS - 1, COLS - 1> submatrix(uint8_t row, uint8_t col) const;

  template <uint8_t R = ROWS, uint8_t C = COLS>
    requires(R > 2 && C > 2)
  float minor(uint8_t row, uint8_t col) const {
    return submatrix(row, col).determinant();
  }

  template <uint8_t R = ROWS, uint8_t C = COLS>
    requires(R > 2 && C > 2)
  float cofactor(uint8_t row, uint8_t col) const {
    return minor(row, col) * (((row + col) % 2) == 0 ? 1 : -1);
  }

  bool isInvertible() const { return determinant() != .0f; }

  Matrix<COLS, ROWS> inverse() const;

  static Matrix identity() {
    static_assert(ROWS == COLS, "Identity matrix must be square");
    Matrix result{};
    for (size_t i = 0; i < ROWS; ++i)
      result(i, i) = 1.0f;
    return result;
  }

  friend std::ostream &operator<<(std::ostream &os, const Matrix &m) {
    os << "{\n";
    for (size_t i = 0; i < ROWS; ++i) {
      os << "  {";
      for (size_t j = 0; j < COLS; ++j) {
        // if (m(i, j) < epsilon) {
        //   os << float(0);
        // } else {
        os << m(i, j);
        // }
        if (j + 1 < COLS)
          os << ", ";
      }
      os << "}";
      if (i + 1 < ROWS)
        os << ",";
      os << "\n";
    }
    os << "}";
    return os;
  }

  // template <uint8_t R = ROWS, uint8_t C = COLS>
  //   requires(R == 4 && C == 4)
  Matrix<4, 4> translate(float x, float y, float z);

  // template <uint8_t R = ROWS, uint8_t C = COLS>
  //   requires(R == 4 && C == 4)
  Matrix<4, 4> scale(float x, float y, float z);
  //   return scaling(x, y, z) * this;
  // }
  //
  // template <uint8_t R = ROWS, uint8_t C = COLS>
  //   requires(R == 4 && C == 4)
  Matrix<4, 4> rotateX(float radians);
  //   return rotationX(radians) * this;
  // }
  //
  // template <uint8_t R = ROWS, uint8_t C = COLS>
  //   requires(R == 4 && C == 4)
  Matrix<4, 4> rotateY(float radians);
  //   return rotationY(radians) * this;
  // }
  //
  // template <uint8_t R = ROWS, uint8_t C = COLS>
  //   requires(R == 4 && C == 4)
  Matrix<4, 4> rotateZ(float radians);
  //   return rotationZ(radians) * this;
  // }
  //
  // template <uint8_t R = ROWS, uint8_t C = COLS>
  //   requires(R == 4 && C == 4)
  Matrix<4, 4> shear(float xy, float xz, float yx, float yz, float zx,
                     float zy);
  //   return shearing(xy, xz, yx, yz, zx, zy) * this;
  // }

private:
  float data_[ROWS][COLS];
};

// Matrix<4, 4> translation(float x, float y, float z);
// Matrix<4, 4> scaling(float x, float y, float z);
// Matrix<4, 4> rotation_x(float radians);
// Matrix<4, 4> rotation_y(float radians);
// Matrix<4, 4> rotation_z(float radians);
// Matrix<4, 4> shearing(float xy, float xz, float yx, float yz, float zx,
//                       float zy);

template <uint8_t ROWS, uint8_t COLS>
Matrix<ROWS, COLS>::Matrix(
    std::initializer_list<std::initializer_list<float>> data) {
  size_t i = 0;
  for (const auto &row : data) {
    if (i >= ROWS) {
      break;
    }
    size_t j = 0;
    for (const auto &value : row) {
      data_[i][j++] = value;
    }
    ++i;
  }
}

template <uint8_t ROWS, uint8_t COLS>
bool Matrix<ROWS, COLS>::operator==(const Matrix &other) const {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < ROWS; j++) {
      if (!epsilonEqual(data_[i][j], other.data_[i][j])) {
        return false;
      }
    }
  }
  return true;
}

template <uint8_t ROWS, uint8_t COLS>
float Matrix<ROWS, COLS>::determinant() const {
  float det = 0.f;
  for (size_t j = 0; j < COLS; j++) {
    det += data_[0][j] * cofactor(0, j);
  }
  return det;
}

template <> inline float Matrix<2, 2>::determinant() const {
  return data_[0][0] * data_[1][1] - data_[0][1] * data_[1][0];
}

template <uint8_t ROWS, uint8_t COLS>
Matrix<ROWS - 1, COLS - 1> Matrix<ROWS, COLS>::submatrix(uint8_t row,
                                                         uint8_t col) const {
  Matrix<ROWS - 1, COLS - 1> subM;
  for (uint8_t i = 0, subI = 0; i < ROWS; i++) {
    if (i == row)
      continue;
    for (uint8_t j = 0, subJ = 0; j < COLS; j++) {
      if (j == col)
        continue;
      subM(subI, subJ) = data_[i][j];
      subJ++;
    }
    subI++;
  }
  return subM;
}

template <uint8_t ROWS, uint8_t COLS>
Matrix<COLS, ROWS> Matrix<ROWS, COLS>::inverse() const {
  if (!isInvertible()) {
    throw std::runtime_error("Matrix is not invertible (determinant is zero)");
  }
  const float invDet = 1.0f / determinant();
  Matrix<COLS, ROWS> inverted;
  for (size_t i = 0; i < COLS; i++) {
    for (size_t j = 0; j < ROWS; j++) {
      inverted(i, j) = cofactor(j, i) * invDet;
    }
  }
  return inverted;
}

using Mat44 = Matrix<4, 4>;
using Mat33 = Matrix<3, 3>;
using Mat22 = Matrix<2, 2>;
