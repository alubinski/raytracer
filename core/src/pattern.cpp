#include "pattern.h"
#include "perlin_noise.h"
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

bool StripePattern::operator==(const PatternPtr &other) const {
  auto o = std::dynamic_pointer_cast<StripePattern>(other);
  return o && BinaryPattern::operator==(other);
}

Color GradientPattern::colorAt(const point_t &point) const {
  const auto gradientA = patternAColorAt(point);
  const auto gradientB = patternBColorAt(point);

  const auto distance = gradientB - gradientA;
  const auto fraction = point.x - std::floor(point.x);

  return gradientA + Color(distance * fraction);
}

bool GradientPattern::operator==(const PatternPtr &other) const {
  auto o = std::dynamic_pointer_cast<GradientPattern>(other);
  return o && BinaryPattern::operator==(other);
}

Color RingPattern::colorAt(const point_t &point) const {
  const float x2 = point.x * point.x;
  const float z2 = point.z * point.z;

  const float magnitude = std::sqrt(x2 + z2);
  return ((int)std::floor(magnitude) % 2) == 0 ? patternAColorAt(point)
                                               : patternBColorAt(point);
}

bool RingPattern::operator==(const PatternPtr &other) const {
  auto o = std::dynamic_pointer_cast<RingPattern>(other);
  return o && BinaryPattern::operator==(other);
}

Color CheckerPattern::colorAt(const point_t &point) const {
  int const x = static_cast<int>(std::floor(point.x));
  int const y = static_cast<int>(std::floor(point.y));
  int const z = static_cast<int>(std::floor(point.z));

  return (x + y + z) % 2 == 0 ? patternAColorAt(point) : patternBColorAt(point);
}

bool CheckerPattern::operator==(const PatternPtr &other) const {
  auto o = std::dynamic_pointer_cast<CheckerPattern>(other);
  return o && BinaryPattern::operator==(other);
}

Color RadialGradientPattern::colorAt(const point_t &point) const {
  const auto gradientA = patternAColorAt(point);
  const auto gradientB = patternBColorAt(point);
  const float x2 = point.x * point.x;
  const float z2 = point.z * point.z;

  const float magnitude = std::sqrt(x2 + z2);
  const Color delta = Color(gradientB - gradientA);

  return gradientA + delta * magnitude;
}

bool RadialGradientPattern::operator==(const PatternPtr &other) const {
  auto o = std::dynamic_pointer_cast<RadialGradientPattern>(other);
  return o && BinaryPattern::operator==(other);
}

Color BlendPattern::colorAt(const point_t &point) const {
  return (patternAColorAt(point) + patternBColorAt(point)) * .5f;
}

bool BlendPattern::operator==(const PatternPtr &other) const {
  auto o = std::dynamic_pointer_cast<BlendPattern>(other);
  return o && BinaryPattern::operator==(other);
}

Color PerlinNoisePattern::colorAt(const point_t &point) const {
  const float distortion = PerlinNoise::noise(point.x, point.y, point.z);
  const auto disturbedPoint =
      Point(point.x + distortion, point.y + distortion, point.z + distortion);
  return pattern_->colorAt(pattern_->transformation() * disturbedPoint);
}

bool PerlinNoisePattern::operator==(const PatternPtr &other) const {
  auto o = std::dynamic_pointer_cast<PerlinNoisePattern>(other);
  return o && pattern_->operator==(other);
}
