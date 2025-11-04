#include "canvas.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

Canvas::Canvas(uint32_t width, uint32_t height)
    : width_(width), height_(height) {
  pixels_ = new Color[width * height];
}

Canvas::Canvas(Canvas &&other)
    : width_(std::exchange(other.width_, 0)),
      height_(std::exchange(other.height_, 0)),
      pixels_(std::exchange(other.pixels_, nullptr)) {}

Color Canvas::operator()(size_t w, size_t h) const {
  return pixels_[w + (h * width_)];
}

uint32_t Canvas::width() const { return width_; }

uint32_t Canvas::height() const { return height_; }

void Canvas::writePixel(size_t w, size_t h, Color color) {
  if (w >= width_ || h >= height_) {
    throw std::out_of_range("Canvas out of range access");
  }
  pixels_[w + h * width_] = color;
}

std::span<Color> Canvas::pixels() const {
  return std::span<Color>(pixels_, pixels_ + width_ * height_);
}

std::string Canvas::getAsPPM() const {
  std::ostringstream oss;
  writePPMHeader(oss);
  writePPMPixels(oss);
  return oss.str();
}

void Canvas::writePPMHeader(std::ostringstream &oss) const {
  oss << "P3" << std::endl
      << width_ << " " << height_ << std::endl
      << "255" << std::endl;
}

void Canvas::writePPMPixels(std::ostringstream &oss) const {
  for (size_t i = 0; i < height_; ++i) {
    writePPMPixelsRow(oss, i);
    oss << std::endl;
  }
}

void Canvas::writePPMPixelsRow(std::ostringstream &oss, size_t row) const {
  uint32_t rowWidth = 0;
  for (size_t j = 0; j < width_; ++j) {
    rowWidth = writePPMPixel(oss, row, j, rowWidth);
  }
}

uint32_t Canvas::writePPMPixel(std::ostringstream &oss, size_t row, size_t col,
                               uint32_t rowWidth) const {
  const Color c = pixels_[col + row * width_];
  rowWidth = writePPMColorComponent(oss, c.r(), rowWidth);
  rowWidth = writePPMColorComponent(oss, c.g(), rowWidth);
  return rowWidth = writePPMColorComponent(oss, c.b(), rowWidth);
}

std::string Canvas::getAsPPMColor(float color) const {
  return std::to_string(std::clamp((int)std::round(color * 255.f), 0, 255));
}

uint32_t Canvas::writePPMColorComponent(std::ostringstream &oss, float value,
                                        uint32_t rowWidth) const {
  constexpr uint32_t PPM_LINE_MAX_WIDTH = 70;
  std::string valueStr = getAsPPMColor(value);

  if (rowWidth + valueStr.length() + 1 > PPM_LINE_MAX_WIDTH) {
    oss << std::endl;
    rowWidth = 0;
  }

  if (rowWidth > 0) {
    oss << ' ';
    rowWidth++;
  }

  oss << valueStr;
  return rowWidth + (uint32_t)valueStr.length();
}

Canvas::~Canvas() {
  if (pixels_) {
    delete[] pixels_;
  }
}
