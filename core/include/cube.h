#include "intersection.h"
#include "shape.h"

class Cube : public Shape {
public:
  std::vector<Intersection> intersept(const Ray &ray) const override;

  vector_t localNormalsAt(const point_t &objectPoint,
                          const Intersection &hit) const override;

private:
  std::pair<float, float> checkAxis(const float origin,
                                    const float direction) const;
};
