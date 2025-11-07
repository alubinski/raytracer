
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
    auto p1 = std::make_shared<StripePattern>(Color(.6f, .6f, .6f),
                                              Color(.2f, .2f, .2f));
    p1->transformation() = rotationY(M_PI / 4.f) * scaling(.25f, 1.f, 1.f);
    auto p2 = std::make_shared<StripePattern>(Color(1.f, .6f, .6f),
                                              Color(.4f, .0f, .0f));
    p2->transformation() = rotationY(M_PI / -4.f) * scaling(.25f, 1.f, 1.f);

    auto pRoot = std::make_shared<CheckerPattern>(p1, p2);
    material.pattern() = pRoot;
    material.setSpecular(0.1f);
  }

  auto roof = std::make_shared<Plane>();
  {
    auto &transformation = roof->transformation();
    transformation = translation(0, 7.5, 0);
    auto &material = roof->material();
    material.pattern() = std::make_shared<RingPattern>(Color{1.f, 0.f, 0.f},
                                                       Color{1.f, .5f, 0.f});
    material.setSpecular(.5f);
  }

  auto backWall = std::make_shared<Plane>();
  {
    backWall->transformation() =
        translation(-10.f, 0.f, 50.f) * rotationX(M_PI / 2.f);
    auto &material = backWall->material();
    material.pattern() = std::make_shared<RadialGradientPattern>(
        Color(1.f, 0.f, 0.f), Color(1.f, 1.f, 0.f));
    material.pattern()->setTransformation(scaling(30.f, 1.f, 15.f));
    material.setAmbient(0.1f);
    material.setSpecular(0.f);
    material.setDiffuse(0.f);
  }

  auto rightWall = std::make_shared<Plane>();
  {
    rightWall->transformation() =
        translation(4.f, 0.f, 0.f) * rotationZ(M_PI / 2.f);
    auto &material = rightWall->material();

    auto p1 =
        std::make_shared<StripePattern>(Color::white(), Color(.0f, .6f, .0f));
    p1->setTransformation(rotationY(M_PI / 2.f) * scaling(.5f, 1.f, 1.f));
    auto p2 =
        std::make_shared<StripePattern>(Color::white(), Color(.0f, .6f, .0f));
    p2->setTransformation(scaling(.5f, 1.f, 1.f));
    auto pRoot = std::make_shared<BlendPattern>(p1, p2);
    pRoot->setTransformation(scaling(.75f, .75f, .75f));
    material.setPattern(pRoot);
    material.setSpecular(0.f);
  }

  auto middle = std::make_shared<Sphere>();
  {

    auto &transformation = middle->transformation();
    transformation = translation(-.5f, 1.f, .5f);
    auto &material = middle->material();
    auto p1 = std::make_shared<StripePattern>(Color{0.f, 1.f, 0.f},
                                              Color{0.f, .25f, 0.f});
    auto pRoot = std::make_shared<PerlinNoisePattern>(p1);
    material.setPattern(pRoot);
    material.setDiffuse(.7f);
    material.setSpecular(.3f);
  }

  auto right = std::make_shared<Sphere>();
  {
    auto &transformation = right->transformation();
    transformation = translation(1.5f, .5f, -.5f) * scaling(.5f, .5f, .5f);
    auto &material = right->material();
    material.setPattern(std::make_shared<GradientPattern>(
        Color(0.f, 0.f, 1.f), Color(1.f, .0f, .0f)));
    material.pattern()->setTransformation(translation(-1.f, 0.f, 0.f) *
                                          scaling(2.f, 1.f, 1.f));
    material.setDiffuse(.7f);
    material.setSpecular(.3f);
  }

  auto left = std::make_shared<Sphere>();
  {
    auto &transformation = left->transformation();
    transformation =
        translation(-1.5f, .33f, -.75f) * scaling(.33f, .33f, .33f);
    auto &material = left->material();
    material.pattern() = std::make_shared<CheckerPattern>(
        Color(1.0f, 1.0f, .0f), Color(.6f, .6f, .0f));
    material.pattern()->setTransformation(scaling(0.5f, 0.5f, 0.5f));
    material.setDiffuse(.7f);
    material.setSpecular(.3f);
  }

  World w{};

  w.addObject(floor);
  w.addObject(backWall);
  w.addObject(rightWall);
  w.addObject(roof);
  w.addObject(middle);
  w.addObject(left);
  w.addObject(right);

  w.addLight(PointLight(Point(-5, 5, -2), Color(1, 1, 1)));

  Camera camera(1300, 600, M_PI / 3.f);
  camera.transform() = view(Point(0, 1.5, -5), Point(0, 1, 0), Vector(0, 1, 0));

  Canvas canvas = camera.render(w);
  writeToFile(canvas, "patterns.ppm");
}
