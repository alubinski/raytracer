
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
#include <iostream>
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
  {
    auto &material = floor->material();
    material.setColor({1.f, .9f, .9f});
    material.setSpecular(0.f);
  }

  auto roof = std::make_shared<Plane>();
  {
    auto &transformation = roof->transformation();
    transformation = translation(0, 10, 0);
    auto &material = roof->material();
    material.setColor({.2f, .9f, .1f});
    material.setSpecular(.1f);
  }

  auto middle = std::make_shared<Sphere>();
  {

    auto &transformation = middle->transformation();
    transformation = translation(-.5f, 1.f, .5f);
    auto &material = middle->material();
    material.setColor({.1, .1, .5});
    material.setDiffuse(.7f);
    material.setSpecular(.3f);
  }

  auto right = std::make_shared<Sphere>();
  {
    auto &transformation = right->transformation();
    transformation = translation(1.5f, .5f, -.5f) * scaling(.5f, .5f, .5f);
    auto &material = right->material();
    material.setColor({.5, .1, .1});
    material.setDiffuse(.7f);
    material.setSpecular(.3f);
  }

  auto left = std::make_shared<Sphere>();
  {
    auto &transformation = left->transformation();
    transformation =
        translation(-1.5f, .33f, -.75f) * scaling(.33f, .33f, .33f);
    auto &material = left->material();
    material.setColor({1.f, .8f, .1});
    material.setDiffuse(.7f);
    material.setSpecular(.3f);
  }

  World w{};

  w.addObject(floor);
  w.addObject(roof);
  w.addObject(middle);
  w.addObject(left);
  w.addObject(right);

  w.addLight(PointLight(Point(-5, 5, -2), Color(1, 1, 1)));

  Camera camera(800, 600, M_PI / 3.f);
  camera.transform() =
      view(Point(5, 2.5, -5), Point(-3, 2.2f, 0), Vector(0, 1, 0));

  std::cout << "hsize: " << camera.hsize() << "\nvsize: " << camera.vsize();
  Canvas canvas = camera.render(w);
  writeToFile(canvas, "plane.ppm");
}
