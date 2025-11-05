
#include "camera.h"
#include "color.h"
#include "material.h"
#include "plane.h"
#include "sphere.h"
#include "transformations.h"
#include "tuple.h"
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

int main() {
  auto floor = std::make_shared<Plane>();
  // floor->setTransformation(scaling(10, 0.01, 10));
  // floor->setMaterial(Material());
  // floor->material().setColor(Color(1, 0.9, 0.9));
  // floor->material().setSpecular(0.f);

  auto wall = std::make_shared<Plane>();
  wall->setTransformation(translation(0, 0, 3) * rotationX(M_PI / 2));
  auto middle = std::make_shared<Sphere>();
  middle->setTransformation(translation(-0.5, 1, 0.5));
  middle->setMaterial(Material());
  middle->material().setColor(Color(0.1, 1, 0.5));
  middle->material().setDiffuse(0.7);
  middle->material().setSpecular(0.3);

  auto right = std::make_shared<Sphere>();
  right->setTransformation(translation(1.5, -0.25, 0.75) *
                           scaling(0.5, 0.5, 0.5));
  right->setMaterial(Material());
  right->material().setColor(Color(0.5, 1, 0.1));
  right->material().setDiffuse(0.7);
  right->material().setSpecular(0.3);

  auto left = std::make_shared<Sphere>();
  left->setTransformation(translation(-1.5, 0.33, -0.75) *
                          scaling(0.33, 0.33, 0.33));
  left->setMaterial(Material());
  left->material().setColor(Color(1, 0.8, 0.1));
  left->material().setDiffuse(0.7);
  left->material().setSpecular(0.3);

  World w{};

  w.addObject(floor);
  // w.addObject(wall);
  w.addObject(middle);
  w.addObject(left);
  w.addObject(right);

  w.addLight(PointLight(Point(-10, 10, -10), Color(1, 1, 1)));

  Camera camera(100, 50, M_PI / 3);
  camera.transform() = view(Point(3, 5, -4), Point(0, 1, 0), Vector(0, 1, 0));

  Canvas canvas = camera.render(w);
  writeToFile(canvas, "plane.ppm");
}
