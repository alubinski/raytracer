#include "canvas.h"
#include "intersection.h"
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
  Canvas canvas{100, 100};
  const auto rayOrigin = Point(0, 0, -5);
  Ray ray{rayOrigin, Vector(0, 0, 0)};
  const float wallZ = 10.f;
  const float wallSize = 7.0f;
  const float pixelSize = wallSize / canvasSize;
  const float wallHalfSize = wallSize * 0.5f;
  auto sphere = std::make_shared<Sphere>();

  std::vector<Mat44> transformations{
      Mat44::identity(), scaling(1, 0.5, 1), scaling(0.5, 1, 1),
      rotationZ(M_PI / 4) * scaling(0.5, 1, 1),
      shearing(1, 0, 0, 0, 0, 0) * scaling(0.5, 1, 1)};
  int i = 0;
  for (auto transform : transformations) {
    sphere->setTransformation(transform);
    for (size_t y = 0; y < canvasSize; y++) {
      const float worldY = wallHalfSize - pixelSize * y;
      for (size_t x = 0; x < canvasSize; x++) {
        const float worldX = -wallHalfSize + x * pixelSize;

        const auto position = Point(worldX, worldY, wallZ);
        const auto r = Ray(rayOrigin, (position - rayOrigin).normalize());
        const auto xs = r.intersept(sphere);

        if (hit(xs)) {
          canvas.writePixel(x, y, Color(1, 0, 0));
        }
      }
    }
    writeToFile(canvas, "sphere_silihoutte_" + std::to_string(++i) + ".ppm");
  }
}
