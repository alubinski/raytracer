#include "color.h"
#include <cstddef>
#include <cstdint>
#include <span>
#include <sstream>
#include <string>
#include <vector>

class Canvas {
public:
  Canvas(uint32_t width, uint32_t height);
  Canvas(Canvas const &other) = delete;

  Color operator()(size_t w, size_t h);

  uint32_t width() const;
  uint32_t height() const;

  void writePixel(size_t w, size_t h, Color color);

  std::span<Color> pixels() const;

  std::string getAsPPM() const;

  ~Canvas();

private:
  void writePPMHeader(std::ostringstream &oss) const;
  void writePPMPixels(std::ostringstream &oss) const;
  void writePPMPixelsRow(std::ostringstream &oss, size_t row) const;
  uint32_t writePPMPixel(std::ostringstream &oss, size_t row, size_t col,
                         uint32_t rowWidth) const;
  std::string getAsPPMColor(float color) const;
  uint32_t writePPMColorComponent(std::ostringstream &oss, float value,
                                  uint32_t rowWidth) const;

  Color *pixels_;
  uint32_t width_;
  uint32_t height_;
};
