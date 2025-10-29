#include "canvas.h"
#include "tuple.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

struct Projectal {
  point_t position;
  vector_t velocity;
};

struct Environment {
  vector_t gravity;
  vector_t wind;
};

void writeToFile(const Canvas &c, std::string filename) {
  const auto ppm = c.getAsPPM();
  std::ofstream ppmfile;
  ppmfile.open(filename);
  ppmfile << ppm;
  ppmfile.close();
}

Projectal tick(Environment env, Projectal proj) {
  proj.position = proj.position + proj.velocity;
  proj.velocity = proj.velocity + env.gravity + env.wind;
  return proj;
}
int main() {
  const auto start = Point(0, 1, 0);
  const auto velocity = Vector(1, 1.8, 0).normalize() * 11.25;
  Projectal p{start, velocity};

  const auto gravity = Vector(0, -0.1, 0);
  const auto wind = Vector(-0.01, 0, 0);
  Environment e{gravity, wind};

  Canvas c(900, 550);
  uint32_t yBias = c.height() - 1;

  int counter = 0;
  do {
    std::cout << "Tick " << ++counter << ":\nPosition:\n\t"
              << "x: " << p.position.x << " y: " << p.position.y
              << " z: " << p.position.z << std::endl;
    std::cout << "\t" << (int)p.position.x << " " << yBias - (int)p.position.y
              << std::endl;
    c.writePixel((int)p.position.x, yBias - (int)p.position.y, Color(0, 1, 0));
    p = tick(e, p);
  } while (p.position.y > 0);

  writeToFile(c, "projectile_trajectory.ppm");
}
