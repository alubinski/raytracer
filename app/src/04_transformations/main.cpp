#include "canvas.h"
#include "transformations.h"
#include "tuple.h"
#include <cstdint>
#include <fstream>
#include <ranges>

void writeToFile(const Canvas &c, std::string filename) {
  const auto ppm = c.getAsPPM();
  std::ofstream ppmfile;
  ppmfile.open(filename);
  ppmfile << ppm;
  ppmfile.close();
}

int main() {
  Canvas canvas{400, 400};
  const Color white{1, 1, 1};

  const float rotationAngle = M_PI / 6.f;
  const Matrix<4, 4> translate = translation(200, 200, 0);
  const Matrix<4, 4> scale = scaling(150, 150, 1);

  canvas.writePixel(200, 200, white);

  const auto twelveOClock = Point(0, 1, 0);
  for (auto hour : std::views::iota(0, 12)) {
    const auto rotation = rotationZ(-rotationAngle * hour);
    const auto hourPoint = translate * scale * rotation * twelveOClock;

    canvas.writePixel((uint32_t)hourPoint.x,
                      canvas.height() - (uint32_t)hourPoint.y, white);
  }
  writeToFile(canvas, "clock.ppm");
}
