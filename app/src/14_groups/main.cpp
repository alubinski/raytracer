
#include "camera.h"
#include "color.h"
#include "cone.h"
#include "cube.h"
#include "cylinder.h"
#include "group.h"
#include "material.h"
#include "plane.h"
#include "sphere.h"
#include "transformations.h"
#include "tuple.h"
#include "types.h"
#include "world.h"
#include <cmath>
#include <fstream>
#include <memory>

void writeToFile(const Canvas &c, std::string filename) {
  const auto ppm = c.getAsPPM();
  std::ofstream ppmfile;
  ppmfile.open(filename);
  ppmfile << ppm;
  ppmfile.close();
}

ShapePtr hexagonEdge() {
  auto edge = std::make_shared<Cylinder>(0, 1, false);
  edge->transformation() = translation(0, 0, -1) * rotationY(-M_PI / 6) *
                           rotationZ(-M_PI / 2) * scaling(.25f, 1.f, .25f);
  return edge;
}

ShapePtr hexagonCorner() {
  auto corner = std::make_shared<Sphere>();
  corner->transformation() = translation(0, 0, -1) * scaling(.25f, .25f, .25f);
  return corner;
}

ShapePtr hexagonSide() {
  auto side = std::make_shared<Group>();
  side->add(hexagonEdge());
  side->add(hexagonCorner());
  return side;
}

ShapePtr hexagon() {
  auto hexagon = std::make_shared<Group>();
  for (auto i{0u}; i < 6; ++i) {
    auto side = hexagonSide();
    side->transformation() = rotationY(i * M_PI / 3);
    hexagon->add(side);
  }
  return hexagon;
}

int main() {
  auto world = World();

  {
    const auto sun = PointLight(Point(0, 5, 0), {1, 1, 1});
    world.addLight(sun);
  }

  world.addObject(hexagon());

  Camera camera(800, 600, M_PI / 3.f);
  camera.transform() =
      view(Point(0.f, 3.f, -3.f), Point(0.f, -1.f, 1.f), Vector(0.f, 1.f, 0.f));

  Canvas canvas = camera.render(world);
  writeToFile(canvas, "groups.ppm");
}
