#include "material.h"
#include "lightning.h"
#include "math.h"
#include <algorithm>

Material::Material()
    : color_(Color(1, 1, 1)), ambient_(0.1f), diffuse_(0.9f), specular_(0.9f),
      shiness_(200.f) {}

Material::Material(Color color, float ambient, float diffuse, float specular,
                   float shiness)
    : color_(color), ambient_(std::clamp(ambient, 0.f, 1.f)),
      diffuse_(std::clamp(diffuse, 0.f, 1.f)),
      specular_(std::clamp(specular, 0.f, 1.f)),
      shiness_(std::clamp(shiness, 10.f, 200.f)) {}

bool Material::operator==(const Material &other) const {
  return color_ == other.color_ && epsilonEqual(ambient_, other.ambient_) &&
         epsilonEqual(diffuse_, other.diffuse_) &&
         epsilonEqual(specular_, other.specular_) &&
         epsilonEqual(shiness_, other.shiness_);
}

bool Material::operator!=(const Material &other) const {
  return !(*this == other);
}
