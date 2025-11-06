#pragma once

#include "color.h"
#include "pattern.h"
#include "tuple.h"
#include <algorithm>

class Material {
public:
  Material();
  Material(Color color, float ambient, float diffuse, float specular,
           float shiness);

  Color color() const;
  float ambient() const { return ambient_; }
  float diffuse() const { return diffuse_; }
  float specular() const { return specular_; }
  float shiness() const { return shiness_; }

  void setColor(const Color &color);

  void setAmbient(float ambient) { ambient_ = std::clamp(ambient, 0.0f, 1.0f); }

  void setDiffuse(float diffuse) { diffuse_ = std::clamp(diffuse, 0.0f, 1.0f); }

  void setSpecular(float specular) {
    specular_ = std::clamp(specular, 0.0f, 1.0f);
  }

  void setShiness(float shiness) {
    shiness_ = std::clamp(shiness, 10.0f, 200.f);
  }

  const PatternPtr &pattern() const { return pattern_; }
  PatternPtr &pattern() { return pattern_; }
  void setPattern(const PatternPtr &pattern) { pattern_ = pattern; }

  Color colorAt(const point_t &p) const { return pattern_->colorAt(p); }

  bool operator==(const Material &other) const;
  bool operator!=(const Material &other) const;

private:
  float ambient_;
  float diffuse_;
  float specular_;
  float shiness_;
  PatternPtr pattern_;
};
