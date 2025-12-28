
#include "group.h"
#include "triangle.h"
#include "tuple.h"
#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

// class NamedGroup : public Group {
// public:
//   NamedGroup(std::string name) : Group(), name_(name) {}
//
//   std::string name() const { return name_; }
//
// private:
//   std::string name_;
// };

struct OBJ_Triangle {
  OBJ_Triangle(point_t point1, point_t point2, point_t point3)
      : p1(point1), p2(point2), p3(point3) {}
  point_t p1;
  point_t p2;
  point_t p3;
};

struct OBJ_Group {
  OBJ_Group(std::string n) : name(n) {}
  std::shared_ptr<Group> toGroup() const {
    auto g = std::make_shared<Group>();
    for (const auto &s : children) {
      g->add(std::make_shared<Triangle>(s.p1, s.p2, s.p3));
    }
    return g;
  }

  std::vector<OBJ_Triangle> children{};
  std::string name;
};

class OBJ_Parser {
public:
  explicit OBJ_Parser(std::istream &is);

  void parse();

  point_t vertex(size_t idx) const;

  const OBJ_Group &defaultGroup() const { return groups_[0]; }

  OBJ_Group *getGroup(const std::string &name) {
    auto it =
        std::find_if(groups_.begin(), groups_.end(),
                     [&name](const OBJ_Group &g) { return g.name == name; });
    return (it != groups_.end()) ? &(*it) : nullptr;
  }

  std::shared_ptr<Group> createRenderableShapes() const {
    auto superGroup = std::make_shared<Group>();
    for (auto &g : groups_) {
      auto group = g.toGroup();
      if (!group->empty()) {
        superGroup->add(group);
      }
    }
    return superGroup;
  }

protected:
  const std::vector<point_t> &vertices();
  uint32_t ignoredLines_{0};
  uint32_t errorLines_{0};
  uint32_t lineNumber_{0};

private:
  std::optional<std::string> parseLine(std::string_view line);
  template <typename T> bool parseNumber(std::string_view &sv, T &out);
  bool parseIdentifier(std::string_view &sv, std::string &out);
  void skipWS(std::string_view &sv);
  void logError(const std::string &msg) const;
  bool parseVertexLine(std::string_view line);
  bool parseFaceLine(std::string_view line);
  bool parseGroupLine(std::string_view line);

  std::istream &input_;
  std::vector<point_t> vertices_;

  // std::shared_ptr<Group> defaultGroup_;
  std::vector<OBJ_Group> groups_;
  std::size_t currGroup = std::size_t(-1);
};
