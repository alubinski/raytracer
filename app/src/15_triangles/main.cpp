
#include "camera.h"
#include "color.h"
#include "cone.h"
#include "cube.h"
#include "cylinder.h"
#include "group.h"
#include "material.h"
#include "obj_parser.h"
#include "pattern.h"
#include "plane.h"
#include "sphere.h"
#include "transformations.h"
#include "triangle.h"
#include "tuple.h"
#include "types.h"
#include "world.h"
#include <cassert>
#include <cmath>
#include <fstream>
#include <memory>
#include <sstream>

void writeToFile(const Canvas &c, std::string filename) {
  const auto ppm = c.getAsPPM();
  std::ofstream ppmfile;
  ppmfile.open(filename);
  ppmfile << ppm;
  ppmfile.close();
}

Material strippedYellowRedMaterial() {
  Material m{};
  auto sp = std::make_shared<StripePattern>(Color::yellow(), Color::red());
  sp->transformation() = scaling(.25f, .25f, .25f) * rotationY(-M_PI / 3);
  m.setPattern(sp);
  return m;
}

Material tealMaterial() {
  Material m{};
  m.setColor(Color(0, 128, 128));
  return m;
}

Material silverMaterial() {
  Material m{};
  m.setColor(Color(192, 192, 192));
  return m;
}

auto plane() {
  auto p = std::make_shared<Plane>();
  p->transformation() = rotationY(M_PI / 2);
  p->setMaterial(strippedYellowRedMaterial());
  return p;
}

auto pyramid() {
  auto p = std::make_shared<Group>();
  point_t apex = Point(0, 1, 0);
  float s2 = SQRT_2 / 2.f;
  auto t1 =
      std::make_shared<Triangle>(Point(-s2, 0, -s2), Point(-s2, 0, s2), apex);
  t1->setMaterial(tealMaterial());
  auto t2 =
      std::make_shared<Triangle>(Point(-s2, 0, s2), Point(s2, 0, s2), apex);
  t2->setMaterial(silverMaterial());
  auto t3 =
      std::make_shared<Triangle>(Point(s2, 0, s2), Point(s2, 0, -s2), apex);
  t3->setMaterial(tealMaterial());
  auto t4 =
      std::make_shared<Triangle>(Point(-s2, 0, -s2), Point(s2, 0, -s2), apex);
  t4->setMaterial(silverMaterial());

  p->add(t1);
  p->add(t2);
  p->add(t3);
  p->add(t4);
  return p;
}

void p1() {
  auto world = World();

  {
    const auto sun = PointLight(Point(-8, 8, 0), {1, 1, 1});
    world.addLight(sun);
  }

  world.addObject(plane());
  world.addObject(pyramid());

  Camera camera(400, 400, M_PI / 4.f);
  camera.transform() =
      view(Point(2, 2, 3), Point(0, 0, 0), Vector(0.f, 1.f, 0.f));

  Canvas canvas = camera.render(world);
  writeToFile(canvas, "chapter15_1.ppm");
}

void p2() {
  auto world = World();

  {
    const auto sun = PointLight(Point(2, 2, -8), {1, 1, 1});
    world.addLight(sun);
  }

  std::ifstream model("teapot.obj");
  assert(model.is_open());
  std::stringstream ss;
  ss << model.rdbuf();
  model.close();

  auto parser = OBJ_Parser(ss);
  parser.parse();
  auto teapot = parser.createRenderableShapes();
  teapot->transformation() =
      translation(0, -1.75, 0) * rotationX(-M_PI_2) * scaling(.25, .25, .25);
  world.addObject(teapot);

  auto p = std::make_shared<Plane>();
  Material m{};
  m.setColor(Color(0, 173, 255));
  m.setReflective(0.6f);
  p->setMaterial(m);
  // world.addObject(p);

  Camera camera(2048, 1024, M_PI / 3.f);
  camera.transform() =
      view(Point(2, 2, -8), Point(0, 0, 0), Vector(0.f, 1.f, 0.f));

  Canvas canvas = camera.render(world);
  writeToFile(canvas, "chapter15_2.ppm");
}

int main() {
  p1();
  p2();
}
