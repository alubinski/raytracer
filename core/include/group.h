#include "shape.h"
#include "types.h"
#include <vector>

class Group : public Shape {
public:
  Group() : Shape{} { updateBounds(); };

  std::vector<Intersection> intersept(const Ray &ray) const override;

  vector_t localNormalsAt(const point_t &objectPoint,
                          const Intersection &hit) const override;

  void add(ShapePtr child);

  const std::vector<ShapePtr> &children() const { return children_; }

  std::vector<ShapePtr> &children() { return children_; }

  bool empty() const { return children_.empty(); }

  bool includes(ShapeConstPtr child) const;

protected:
  virtual void updateBounds() override;

private:
  std::vector<ShapePtr> children_;
};
