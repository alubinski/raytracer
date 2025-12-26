
#include "camera.h"
#include "color.h"
#include "cone.h"
#include "cube.h"
#include "cylinder.h"
#include "material.h"
#include "plane.h"
#include "sphere.h"
#include "transformations.h"
#include "tuple.h"
#include "world.h"
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
  auto world = World();

  {
    const auto sun = PointLight(Point(0, 18, 0), {1, 1, 1});
    world.addLight(sun);
  }

  {
    auto mirror = std::make_shared<Cube>();
    mirror->setTransformation(translation(-29.9f, 10.f, 0.f) *
                              rotationZ(M_PI * .5f) * scaling(6.f, .1f, 20.f));
    auto &m = mirror->material();
    m.setColor(Color::black());
    m.setShiness(300.f);
    m.setReflective(1.f);
    world.addObject(mirror);
  }

  {
    auto floorAndRoof = std::make_shared<Cube>();
    floorAndRoof->setTransformation(translation(0.f, 10.f, 0.f) *
                                    scaling(100.f, 10.f, 100.f));
    auto &m = floorAndRoof->material();
    m.setSpecular(0.1f);
    m.setAmbient(.5f);
    auto p =
        std::make_shared<CheckerPattern>(Color::black(), Color(.6f, .6f, .6f));
    p->setTransformation(scaling(.05f, .05f, .05f));
    m.setPattern(p);
    world.addObject(floorAndRoof);
  }

  {
    auto walls = std::make_shared<Cube>();
    walls->setTransformation(translation(0.f, 10.f, 0.f) *
                             scaling(30.f, 100.f, 30.f));
    auto &m = walls->material();
    m.setSpecular(0.1f);
    m.setAmbient(.5f);
    auto p = std::make_shared<StripePattern>(Color(.6f, .3f, .1f),
                                             Color(.5f, .2f, .05f));
    p->setTransformation(scaling(.1f, .1f, .1f));
    m.setPattern(p);
    world.addObject(walls);
  }

  {
    auto lampBody = std::make_shared<Cylinder>();
    lampBody->setTransformation(translation(0, 18, 0) * translation(0, 2, 0));
    lampBody->minimum() = -1;
    lampBody->maximum() = 0;
    lampBody->closed() = true;

    auto &m = lampBody->material();
    m.setColor(Color::yellow());
    m.setReflective(0.1f);
    world.addObject(lampBody);
  }

  {
    auto lampHat = std::make_shared<Cone>();
    lampHat->setTransformation(translation(0, 18, 0) * translation(0, 1.35, 0) *
                               scaling(2.5, 1, 2.5));
    lampHat->minimum() = -1.5;
    lampHat->maximum() = -0.25;
    lampHat->closed() = true;

    auto &m = lampHat->material();
    m.setColor(Color::yellow());
    m.setReflective(0.1f);
    world.addObject(lampHat);
  }

  {
    auto lampBulb = std::make_shared<Sphere>();
    lampBulb->setTransformation(translation(0, 18, 0) * scaling(.4, .6, .4));
    lampBulb->setCastsShadows(false);

    auto &m = lampBulb->material();
    m.setColor(Color::white());
    m.setAmbient(1.f);
    world.addObject(lampBulb);
  }

  {
    std::vector<std::pair<float, float>> legsPos = {
        {-10.f, -8.f},
        {0.f, -8.f},
        {0.f, 8.f},
        {-10.f, 8.f},
    };
    for (auto pos2d : legsPos) {
      auto leg = std::make_shared<Cube>();
      leg->setTransformation(translation(pos2d.first, 3.f, pos2d.second) *
                             scaling(.25f, 3.f, .25f));
      leg->material().setColor(Color::red());
      leg->material().setAmbient(.25f);
      world.addObject(leg);
    }

    auto top = std::make_shared<Cube>();
    top->setTransformation(translation(-5.f, 6.25f, 0.f) *
                           scaling(5.25f, .5f, 8.25f));
    top->material().setColor(Color::red());
    top->material().setAmbient(.25f);
    world.addObject(top);
  }

  Camera camera(1362, 638, M_PI / 3.f);
  camera.transform() = view(Point(29.f, 10.f, -29.f), Point(-5.f, 9.f, 0.f),
                            Vector(0.f, 1.f, 0.f));

  Canvas canvas = camera.render(world);
  writeToFile(canvas, "cylinders.ppm");
}
