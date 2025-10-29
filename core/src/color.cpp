#include "color.h"

Color operator*(const Color &lhs, const Color &rhs) {
  return Color(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b);
}
