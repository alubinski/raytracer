#include "canvas.h"
#include "color.h"
#include "intersection.h"
#include "lightning.h"
#include "ray.h"
#include "sphere.h"
#include "transformations.h"
#include "tuple.h"
#include <cstdint>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

void writeToFile(const Canvas &c, std::string filename) {
  const auto ppm = c.getAsPPM();
  std::ofstream ppmfile;
  ppmfile.open(filename);
  ppmfile << ppm;
  ppmfile.close();
}

int main() {
  const uint32_t canvasSize = 100;
  const auto rayOrigin = Point(0, 0, -5);
  Ray ray{rayOrigin, Vector(0, 0, 0)};
  const float wallZ = 10.f;
  const float wallSize = 7.0f;
  const float pixelSize = wallSize / canvasSize;
  const float wallHalfSize = wallSize * 0.5f;
  auto sphere = std::make_shared<Sphere>();
  sphere->material().setColor(Color(1, 0.2, 1));
  const auto lightPosition = Point(-10, 10, -10);
  const auto lightColor = Color::white();
  const auto light = PointLight(lightPosition, lightColor);

  std::vector<Mat44> transformations{
      Mat44::identity(), scaling(1, 0.5, 1), scaling(0.5, 1, 1),
      rotationZ(M_PI / 4) * scaling(0.5, 1, 1),
      shearing(1, 0, 0, 0, 0, 0) * scaling(0.5, 1, 1)};
  int i = 0;
  for (auto transform : transformations) {
    Canvas canvas{canvasSize, canvasSize};
    sphere->setTransformation(transform);
    for (size_t y = 0; y < canvasSize; y++) {
      const float worldY = wallHalfSize - pixelSize * y;
      for (size_t x = 0; x < canvasSize; x++) {
        const float worldX = -wallHalfSize + x * pixelSize;

        const auto position = Point(worldX, worldY, wallZ);
        const auto r = Ray(rayOrigin, (position - rayOrigin).normalize());

        if (const auto xs = r.intersept(sphere)) {
          const auto xsVec = to_vector(xs.value());
          if (const auto h = hit(to_vector(xs.value()))) {
            const auto point = r.position(h->t());
            const auto normal = h->object()->normalsAt(point);
            const auto eye = -r.direction();
            const auto color =
                lightining(h->object()->material(), light, point, eye, normal);
            canvas.writePixel(x, y, color);
          }
        }
      }
    }
    writeToFile(canvas, "sphere_" + std::to_string(++i) + ".ppm");
  }
}
