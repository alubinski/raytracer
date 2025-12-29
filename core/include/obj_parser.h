
#include "group.h"
#include "smooth_triangle.h"
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

struct FaceVertex {
  std::size_t v = 0;
  std::size_t n = 0;

  bool operator==(const FaceVertex &other) const {
    return v == other.v && n == other.n;
  }
};

struct OBJ_Triangle {
  OBJ_Triangle(FaceVertex face1, FaceVertex face2, FaceVertex face3)
      : f1(face1), f2(face2), f3(face3) {}

  bool operator==(const OBJ_Triangle &other) const {
    return f1 == other.f1 && f2 == other.f2 && f3 == other.f3;
  }

  std::shared_ptr<Shape> toShape(const std::vector<point_t> &vertices,
                                 const std::vector<vector_t> &normals) const {
    if (f1.n != 0 && f2.n != 0 && f2.n != 0) {
      return std::make_shared<SmoothTriangle>(vertices[f1.n], vertices[f2.n],
                                              vertices[f3.n], normals[f1.n],
                                              normals[f2.n], normals[f3.n]);
    }
    return std::make_shared<Triangle>(vertices[f1.n], vertices[f2.n],
                                      vertices[f3.n]);
  }

  size_t p1() const { return f1.v; }
  size_t p2() const { return f2.v; }
  size_t p3() const { return f3.v; }
  size_t n1() const { return f1.n; }
  size_t n2() const { return f2.n; }
  size_t n3() const { return f3.n; }

  FaceVertex f1;
  FaceVertex f2;
  FaceVertex f3;

  // OBJ_Parser &parser;

  // OBJ_Triangle(point_t point1, point_t point2, point_t point3, vector_t
  // normal1,
  //              vector_t normal2, vector_t normal3)
  //     : p1(point1), p2(point2), p3(point3), n1(normal1), n2(normal2),
  //       n3(normal3) {}
  //
  // bool operator==(const OBJ_Triangle &other) {
  //   return p1 == other.p1 && p2 == other.p2 && p3 == other.p3 &&
  //          n1 == other.n1 && n2 == other.n2 && n3 == other.n3;
  // }
  //
  // point_t p1;
  // point_t p2;
  // point_t p3;
  // vector_t n1;
  // vector_t n2;
  // vector_t n3;
};

struct OBJ_Group {
  OBJ_Group(std::string n) : name(n) {}
  std::shared_ptr<Group> toGroup(const std::vector<point_t> &vertices,
                                 const std::vector<vector_t> &normals) const {
    auto g = std::make_shared<Group>();
    for (auto &s : children) {

      g->add(s.toShape(vertices, normals));
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
  vector_t normal(size_t idx) const;

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
      auto group = g.toGroup(vertices_, normals_);
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
  bool parseNormalLine(std::string_view line);
  bool parseFaceLine(std::string_view line);
  bool parseGroupLine(std::string_view line);

  std::istream &input_;
  std::vector<point_t> vertices_;
  std::vector<vector_t> normals_;

  // std::shared_ptr<Group> defaultGroup_;
  std::vector<OBJ_Group> groups_;
  std::size_t currGroup = std::size_t(-1);
};
