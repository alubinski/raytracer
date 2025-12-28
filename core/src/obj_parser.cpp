#include "obj_parser.h"
#include "triangle.h"
#include <cstdint>
#include <format>
#include <iostream>
#include <istream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

OBJ_Parser::OBJ_Parser(std::istream &is) : input_(is) {
  groups_.push_back(OBJ_Group("default"));
  currGroup = 0;
}

void OBJ_Parser::parse() {
  std::string line;
  uint32_t lineNumber{0};
  while (std::getline(input_, line, '\n')) {
    lineNumber_++;

    auto err = parseLine(line);
    if (err) {
      errorLines_++;
      logError(*err);
    }
  }

  std::cerr << std::format("Parser skipped {} lines\n", ignoredLines_);
}

std::optional<std::string> OBJ_Parser::parseLine(std::string_view line) {

  if (line.empty()) {
    ignoredLines_++;
    return "empty line";
  }

  char prefix{line.front()};
  line.remove_prefix(1);

  switch (prefix) {
  case 'v': {
    if (parseVertexLine(line)) {
      return {};
    }
    return "invalid vertex";
  }
  case 'f': {
    if (parseFaceLine(line)) {
      return {};
    }
    return "invalid face";
  }
  case 'g': {
    if (parseGroupLine(line)) {
      return {};
    }
    return "invalid group name";
  }
  default: {
    ignoredLines_++;
    return std::string("unknown prefix '") + prefix + "'";
  }
  }
}

template <typename T>
bool OBJ_Parser::parseNumber(std::string_view &sv, T &out) {
  static_assert(std::is_arithmetic_v<T>,
                "parse_number requires arithmetic type");
  skipWS(sv);

  const char *begin = sv.data();
  const char *end = begin + sv.size();

  auto [ptr, ec] = std::from_chars(begin, end, out);
  if (ec != std::errc{})
    return false;

  sv.remove_prefix(ptr - begin);
  return true;
}

bool OBJ_Parser::parseIdentifier(std::string_view &sv, std::string &out) {
  skipWS(sv);
  if (sv.empty())
    return false;

  std::size_t len = 0;
  while (len < sv.size() && !std::isspace(static_cast<unsigned char>(sv[len])))
    ++len;

  out.assign(sv.substr(0, len));
  sv.remove_prefix(len);
  return true;
}

void OBJ_Parser::skipWS(std::string_view &sv) {
  while (!sv.empty() && std::isspace(static_cast<unsigned char>(sv.front())))
    sv.remove_prefix(1);
}

void OBJ_Parser::logError(const std::string &msg) const {
  std::cerr << "Line " << lineNumber_ << ": " << msg << "\n";
}

point_t OBJ_Parser::vertex(size_t idx) const {
  if (idx == 0 || idx > vertices_.size())
    throw std::out_of_range("vertex index out of range (1-based)");

  return vertices_[idx - 1];
}

bool OBJ_Parser::parseVertexLine(std::string_view line) {
  float x, y, z;
  if (parseNumber(line, x) && parseNumber(line, y) && parseNumber(line, z)) {
    skipWS(line);
    if (line.empty()) {
      vertices_.push_back(Point(x, y, z));
      return true;
    }
  }
  return false;
}

bool OBJ_Parser::parseFaceLine(std::string_view line) {
  std::vector<size_t> indices;

  while (true) {
    std::size_t idx;
    if (!parseNumber(line, idx))
      break;

    if (idx == 0 || idx > vertices_.size())
      return false;

    indices.push_back(idx);
  }

  if (indices.size() < 3) {
    return false;
  }

  const auto p1 = vertex(indices[0]);

  for (size_t i{1}; i < indices.size() - 1; ++i) {

    const auto p2 = vertex(indices[i]);
    const auto p3 = vertex(indices[i + 1]);
    groups_[currGroup].children.push_back(OBJ_Triangle(p1, p2, p3));
  }

  skipWS(line);
  return line.empty();
}

bool OBJ_Parser::parseGroupLine(std::string_view line) {
  std::string name;
  if (!parseIdentifier(line, name))
    return false;

  skipWS(line);
  if (!line.empty())
    return false;

  groups_.push_back(OBJ_Group{name});
  currGroup = groups_.size() - 1;
  return true;
}
