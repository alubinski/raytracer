
#include "color.h"
#include "tuple.h"
#include <memory>

class Pattern;
using PatternPtr = std::shared_ptr<Pattern>;

class Pattern {
public:
  virtual Color colorAt(const point_t &point) const = 0;
  virtual bool operator==(const PatternPtr &other) const = 0;
};

class SolidPattern : public Pattern {
public:
  SolidPattern(const Color &color) : color_(color) {}

  Color colorAt(const point_t &point) const override;
  bool operator==(const PatternPtr &other) const override;
  Color color() const { return color_; }

private:
  Color color_;
};

class BinaryPattern : public Pattern {
public:
  BinaryPattern(PatternPtr patternA, PatternPtr patternB)
      : patternA_(patternA), patternB_(patternB) {}
  Color colorA() const;
  Color colorB() const;

protected:
  Color patternAColorAt(const point_t &point) const;
  Color patternBColorAt(const point_t &point) const;
  bool operator==(const PatternPtr &other) const override;

private:
  PatternPtr patternA_;
  PatternPtr patternB_;
};

#define DECLARE_BINARY_PATTERN(className)                                      \
  class className : public BinaryPattern {                                     \
  public:                                                                      \
    className(const Color &colorA, const Color &colorB)                        \
        : BinaryPattern(std::make_shared<SolidPattern>(colorA),                \
                        std::make_shared<SolidPattern>(colorB)) {}             \
    Color colorAt(const point_t &point) const override;                        \
    bool operator==(const PatternPtr &other) const override;                   \
  };

DECLARE_BINARY_PATTERN(StripePattern)

#undef DECLARE_BINARY_PATTERN
