#include "group.h"
#include "ranges"
#include "ray.h"
#include "types.h"
#include <algorithm>
#include <stdexcept>
#include <vector>

void Group::add(ShapePtr child) {
  children_.push_back(child);
  child->setParent(shared_from_this());
}

bool Group::includes(ShapeConstPtr child) const {
  return std::find(children_.begin(), children_.end(), child) !=
         children_.end();
}

std::vector<Intersection> Group::intersept(const Ray &ray) const {
  const Ray r = ray * transformation().inverse();
  auto all = children_ | std::views::transform([&](const auto &child) {
               return r.intersept(child);
             }) |
             std::views::join;

  std::vector<Intersection> xs;
  std::ranges::copy(all, std::back_inserter(xs));

  std::sort(xs.begin(), xs.end());
  return xs;
}

Tuple Group::localNormalsAt(const Tuple &) const {
  throw std::logic_error(
      "Groups don't have normal by themselves. NormalAtLocal must "
      "be called directly on contained shapes.");
}
