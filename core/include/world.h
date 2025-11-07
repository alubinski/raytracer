#pragma once

#include "computation.h"
#include "lightning.h"
#include "tuple.h"
#include "types.h"
#include <cstdint>
#include <vector>

class Ray;

class World {
public:
  World() = default;
  ~World() = default;

  static World defaultWorld();

  void addObject(const ShapePtr &object);
  void removeObject(const ShapePtr &object);
  const std::vector<ShapePtr> &objects() const { return objects_; }
  std::vector<ShapePtr> &objects() { return objects_; }
  bool containsObject(const ShapePtr &object) const;

  void addLight(const PointLight &light);
  void removeLight(const PointLight &light);
  const std::vector<PointLight> &lights() const { return lights_; }
  std::vector<PointLight> &lights() { return lights_; }
  bool containsLight(const PointLight &light) const;

  Color shadeHit(const ComputationData &comps,
                 uint8_t recursionLimit = 4u) const;

  Color reflectedColor(const ComputationData &comps,
                       uint8_t recursionLimit = 4u) const;

  Color reflactedColor(const ComputationData &comps,
                       uint8_t recursionLimit = 4u) const;

  Color colorAt(const Ray &r, uint8_t recursionLimit = 4u) const;

  bool isShadowed(point_t point) const;

  void clear();
  bool isEmpty() const;

private:
  std::vector<ShapePtr> objects_;
  std::vector<PointLight> lights_;
};
