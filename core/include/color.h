#include "tuple.h"
#include <vector>

struct Color : public Tuple {
public:
  Color() : Tuple(0, 0, 0, 0), r(x), g(y), b(z) {}
  Color(float red, float green, float blue)
      : Tuple(red, green, blue, 0), r(x), g(y), b(z) {}
  Color(Tuple const &other)
      : Tuple(other.x, other.y, other.z, other.w), r(x), g(y), b(z) {}

  Color &operator=(const Color &other) {
    if (this == &other)
      return *this;

    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;

    r = x;
    g = y;
    b = z;
    return *this;
  }

  float &r;
  float &g;
  float &b;

private:
  using Tuple::w;
  using Tuple::x;
  using Tuple::y;
  using Tuple::z;
};

// Hadamard product
Color operator*(const Color &lhs, const Color &rhs);
