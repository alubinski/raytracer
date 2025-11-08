
#include "camera.h"
#include "color.h"
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

  const point_t sunPos = Point(-50.f, 15.f, 99.f);
  world.addLight(PointLight(sunPos, {1.f, 1.f, 1.f}));

  {
    auto water = std::make_shared<Plane>();
    water->setCastsShadows(false);
    auto &m = water->material();
    auto p1 =
        std::make_shared<StripePattern>(Color::white(), Color::blue() * .1f);
    p1->transformation() = rotationY(M_PI / 2.f);
    m.pattern() = std::make_shared<PerlinNoisePattern>(p1);
    m.setDiffuse(.1f);
    m.setSpecular(.9f);
    m.setShiness(200.f);
    m.setReflective(1.f);
    m.setTransparency(1.f);
    m.setReflectiveIndex(Material::GetIndexOfRefraction(Material::Type::Water));
    world.addObject(water);
  }

  {
    auto sand = std::make_shared<Plane>();
    sand->transformation() = rotationX(M_PI / 16.f);
    auto &m = sand->material();
    m.setPattern(std::make_shared<CheckerPattern>(Color(.85f, .80f, .50f),
                                                  Color(.99f, .99f, .61f)));
    m.setAmbient(.3f);
    m.setDiffuse(1.f);
    m.setSpecular(0.f);
    world.addObject(sand);
  }

  {
    auto sky = std::make_shared<Plane>();
    sky->transformation() =
        translation(0.f, 0.f, 100.f) * rotationX(M_PI / 2.f);
    auto &m = sky->material();
    m.setColor({.6f, .6f, 1.f});
    m.setAmbient(1.f);
    m.setDiffuse(0.f);
    m.setSpecular(0.f);
    world.addObject(sky);
  }

  {
    auto sun = std::make_shared<Sphere>();
    sun->setCastsShadows(false);
    sun->transformation() =
        translation(sunPos.x, sunPos.y, sunPos.z) * scaling(4.f, 4.f, 4.f);
    auto &m = sun->material();
    m.setColor(Color::yellow());
    m.setAmbient(1.f);
    m.setSpecular(0.f);
    m.setDiffuse(0.f);
    world.addObject(sun);
  }

  {
    auto r1 = std::make_shared<Sphere>();
    r1->transformation() =
        translation(0.f, -.3f, .75f) * scaling(.5f, .25f, .35f);
    r1->material().setPattern(std::make_shared<GradientPattern>(
        Color(.2f, .4f, .1f), Color::black()));
    r1->material().setAmbient(.2f);
    world.addObject(r1);

    auto r2 = std::make_shared<Sphere>();
    r2->transformation() =
        translation(1.f, -.3f, .6f) * scaling(.2f, .5f, .25f);
    r2->material().setPattern(std::make_shared<GradientPattern>(
        Color(.8f, .4f, .2f), Color::white()));
    r2->material().setAmbient(.2f);
    world.addObject(r2);
  }

  Camera camera(1362, 638, M_PI / 3.f);
  camera.transform() =
      view(Point(0.f, 1.f, -3.5f), Point(0.f, 0.f, 5.f), Vector(0.f, 1.f, 0.f));

  Canvas canvas = camera.render(world);
  writeToFile(canvas, "fresnel_effect.ppm");
}
