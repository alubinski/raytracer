#include "canvas.h"
#include <catch2/catch.hpp>

TEST_CASE("canvas - Constructor") {
  const Canvas c = Canvas(10, 20);
  REQUIRE(c.width() == 10);
  REQUIRE(c.height() == 20);
  for (auto &v : c.pixels()) {
    REQUIRE(v == Color(0, 0, 0));
  }
}

TEST_CASE("canvas - writePixel") {
  Canvas c = Canvas(10, 20);
  const Color red = Color(1, 0, 0);
  c.writePixel(2, 3, red);
  REQUIRE(c(2, 3) == red);
};

TEST_CASE("canvas - getAsPPM") {
  SECTION("writePPMHeader") {
    const auto c = Canvas(5, 3);
    REQUIRE(c.getAsPPM().substr(0, 11) == "P3\n5 3\n255\n");
  }

  SECTION("writePPMPixels") {
    constexpr char const *expected = "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
                                     "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0\n"
                                     "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255\n";

    auto canvas = Canvas(5, 3);
    const auto c1 = Color(1.5, 0, 0);
    const auto c2 = Color(0, 0.5, 0);
    const auto c3 = Color(-0.5, 0, 1);
    canvas.writePixel(0, 0, c1);
    canvas.writePixel(2, 1, c2);
    canvas.writePixel(4, 2, c3);

    REQUIRE(canvas.getAsPPM().substr(11) == expected);
  }

  SECTION("Split long lines in PPM file") {
    constexpr char const *expected =
        "P3\n10 2\n255\n"
        "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
        "153 255 204 153 255 204 153 255 204 153 255 204 153\n"
        "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
        "153 255 204 153 255 204 153 255 204 153 255 204 153\n";

    auto canvas = Canvas(10, 2);
    for (int i = 0; i < canvas.width(); ++i) {
      for (int j = 0; j < canvas.height(); ++j) {
        canvas.writePixel(i, j, Color(1, 0.8, 0.6));
      }
    }

    REQUIRE(canvas.getAsPPM() == expected);
  }

  SECTION("PPM file is terminated by newline character") {
    const auto canvas = Canvas(5, 3);
    const auto ppm = canvas.getAsPPM();
    REQUIRE(ppm.at(ppm.size() - 1) == '\n');
  }
}
