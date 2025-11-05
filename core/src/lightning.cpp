#include "lightning.h"
#include "color.h"
#include "tuple.h"
#include <cmath>
#include <iostream>

PointLight::PointLight(const point_t position, const Color intensity)
    : position_(position), intensity_(intensity) {}

bool PointLight::operator==(const PointLight &other) const {
  return position_ == other.position() && intensity_ == other.intensity();
}

Color lightining(const Material &material, const PointLight &light,
                 const point_t &position, const vector_t &eyeVec,
                 const vector_t &normalVec, bool inShadow) {
  // combine the surface color with the light's color/intensity
  const auto effectiveColor = material.color() * light.intensity();

  // find the direction to the light source
  const auto lightVec = (light.position() - position).normalize();

  // compute the ambient contribution
  const auto ambient = Color(effectiveColor * material.ambient());
  if (inShadow) {
    return ambient;
  }

  // light_dot_normal represents the cosine of the angle between the
  // light vector and the normal vector. A negative number means the
  // light is on the other side of the surface.
  const auto lightDotNormal = dotProduct(lightVec, normalVec);
  Color diffuse, specular;
  if (lightDotNormal < 0.f) {
    diffuse = Color::black();
    specular = Color::black();
  } else {
    // compute the diffuse contribution
    diffuse = effectiveColor * material.diffuse() * lightDotNormal;

    // reflect_dot_eye represents the cosine of the angle between the
    // reflection vector and the eye vector. A negative number means the
    // light reflects away from the eye.
    const auto reflectVec = (-lightVec).reflect(normalVec);
    const auto reflectDotEye = dotProduct(reflectVec, eyeVec);

    if (reflectDotEye < 0.f) {
      specular = Color::black();
    } else {
      // compute the specular contribution
      const auto factor = std::powf(reflectDotEye, material.shiness());
      specular = light.intensity() * material.specular() * factor;
    }
  }
  // Add the three contributions together to get the final shading

  return ambient + diffuse + specular;
}
