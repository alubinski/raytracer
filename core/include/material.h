#pragma once

#include "color.h"
#include <algorithm>

class Material {
public:
  Material();
  Material(Color color, float ambient, float diffuse, float specular,
           float shiness);

  Color color() const { return color_; }
  float ambient() const { return ambient_; }
  float diffuse() const { return diffuse_; }
  float specular() const { return specular_; }
  float shiness() const { return shiness_; }

  void setColor(const Color &color) { color_ = color; }

  void setAmbient(float ambient) { ambient_ = std::clamp(ambient, 0.0f, 1.0f); }

  void setDiffuse(float diffuse) { diffuse_ = std::clamp(diffuse, 0.0f, 1.0f); }

  void setSpecular(float specular) {
    specular_ = std::clamp(specular, 0.0f, 1.0f);
  }

  void setShiness(float shiness) {
    shiness_ = std::clamp(shiness, 10.0f, 200.f);
  }

  bool operator==(const Material &other) const;
  bool operator!=(const Material &other) const;

private:
  Color color_;
  float ambient_;
  float diffuse_;
  float specular_;
  float shiness_;
};
