#include "pattern.h"
#include "shape.h"
#include "tuple.h"
#include <cmath>
#include <memory>

Color Pattern::colorAtObject(const ShapeConstPtr &shape,
                             const point_t &worldPoint) const {
  const point_t objectPoint = shape->transformation().inverse() * worldPoint;
  const point_t patternPoint = transformation().inverse() * objectPoint;
  return colorAt(patternPoint);
}

Color SolidPattern::colorAt(const point_t &point) const { return color_; }

bool SolidPattern::operator==(const PatternPtr &other) const {
  auto o = std::dynamic_pointer_cast<SolidPattern>(other);
  return o && color_ == o->color_;
}

Color BinaryPattern::colorA() const {
  if (auto solid = std::dynamic_pointer_cast<SolidPattern>(patternA_))
    return solid->color();

  throw std::runtime_error(
      "BinaryPattern::colorA() called on non-SolidPattern subpatternA.");
}

Color BinaryPattern::colorB() const {
  if (auto solid = std::dynamic_pointer_cast<SolidPattern>(patternB_))
    return solid->color();

  throw std::runtime_error(
      "BinaryPattern::colorA() called on non-SolidPattern subpatternA.");
}

Color BinaryPattern::patternAColorAt(const point_t &point) const {
  return patternA_->colorAt(point);
}

Color BinaryPattern::patternBColorAt(const point_t &point) const {
  return patternB_->colorAt(point);
}

bool BinaryPattern::operator==(const PatternPtr &other) const {
  auto o = std::dynamic_pointer_cast<BinaryPattern>(other);
  return o && patternA_ == o->patternA_ && patternB_ == o->patternB_;
}

Color StripePattern::colorAt(const point_t &point) const {
  return ((int)std::floor(point.x) % 2) ? patternBColorAt(point)
                                        : patternAColorAt(point);
}

bool GradientPattern::operator==(const PatternPtr &other) const {
  auto o = std::dynamic_pointer_cast<GradientPattern>(other);
  return o && BinaryPattern::operator==(other);
}

Color GradientPattern::colorAt(const point_t &point) const {
  const auto gradientA = patternAColorAt(point);
  const auto gradientB = patternBColorAt(point);

  const auto distance = gradientB - gradientA;
  const auto fraction = point.x - std::floor(point.x);

  return gradientA + Color(distance * fraction);
}

bool StripePattern::operator==(const PatternPtr &other) const {
  auto o = std::dynamic_pointer_cast<StripePattern>(other);
  return o && BinaryPattern::operator==(other);
}
