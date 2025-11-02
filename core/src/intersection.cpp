#include "intersection.h"

std::optional<const Intersection> hit(const std::vector<Intersection> &xs) {
  auto it = std::find_if(xs.begin(), xs.end(),
                         [](const Intersection &i) { return i.t() > 0; });
  if (it != xs.end())
    return *it; // return a reference wrapper
  else
    return std::nullopt;
}
