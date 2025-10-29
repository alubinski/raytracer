#include "tuple.h"
#include <iostream>

struct Projectal {
  point_t position;
  vector_t velocity;
};

struct Environment {
  vector_t gravity;
  vector_t wind;
};

Projectal tick(Environment env, Projectal proj) {
  proj.position = proj.position + proj.velocity;
  proj.velocity = proj.velocity + env.gravity + env.wind;
  return proj;
}

int main() {
  Projectal p = Projectal{Point(0, 1, 0), 2 * Vector(1, 1, 0).normalize()};
  Environment e = Environment{Vector(0, -0.1, 0), Vector(-0.01, 0, 0)};

  int counter = 0;
  while (p.position.y > 0) {
    std::cout << "Tick " << ++counter << ":\nPosition:\n\t"
              << "x: " << p.position.x << " y: " << p.position.y
              << " z: " << p.position.z << std::endl;
    p = tick(e, p);
  }
}
