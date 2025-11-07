#include "material.h"
#include "lightning.h"
#include "math.h"
#include "tuple.h"
#include <algorithm>
#include <memory>

Material::Material()
    : ambient_(0.1f), diffuse_(0.9f), specular_(0.9f), shiness_(200.f),
      refletive_(0.f), transparency_(0.f), reflectiveIndex_(1.f),
      pattern_(std::make_shared<SolidPattern>(Color(1, 1, 1))) {}

Material::Material(Color color, float ambient, float diffuse, float specular,
                   float shiness, float reflective, float transparency,
                   float reflectiveIndex)
    : ambient_(std::clamp(ambient, 0.f, 1.f)),
      diffuse_(std::clamp(diffuse, 0.f, 1.f)),
      specular_(std::clamp(specular, 0.f, 1.f)),
      refletive_(std::clamp(reflective, 0.f, 1.f)),
      shiness_(std::clamp(shiness, 10.f, 200.f)), transparency_(transparency),
      reflectiveIndex_(reflectiveIndex),
      pattern_(std::make_shared<SolidPattern>(Color(1, 1, 1))) {}

// sampling for backward compatibility
Color Material::color() const { return pattern_->colorAt(Point(0, 0, 0)); }

void Material::setColor(const Color &color) {
  pattern_ = std::make_shared<SolidPattern>(color);
}

bool Material::operator==(const Material &other) const {
  return color() == other.color() && epsilonEqual(ambient_, other.ambient_) &&
         epsilonEqual(diffuse_, other.diffuse_) &&
         epsilonEqual(specular_, other.specular_) &&
         epsilonEqual(shiness_, other.shiness_);
}

bool Material::operator!=(const Material &other) const {
  return !(*this == other);
}
