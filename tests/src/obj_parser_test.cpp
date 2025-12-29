#include "obj_parser.h"
#include "tuple.h"
#include <catch2/catch.hpp>
#include <cstdint>
#include <memory>
#include <sstream>
#include <string>

class Test_OBJ_Parser : public OBJ_Parser {
public:
  Test_OBJ_Parser(std::stringstream &ss) : OBJ_Parser(ss) {}

  uint32_t ignoredLines() const { return ignoredLines_; }
};

TEST_CASE("obj_parser - ignore unrecognized lines") {
  auto ss =
      std::stringstream(std::string("There was a young lady named Bright\n"
                                    "who traveled much faster than light.\n"
                                    "She set out one day\n"
                                    "in a relative way,\n"
                                    "and came back the previous night.\n"));
  Test_OBJ_Parser parser(ss);
  parser.parse();
  REQUIRE(parser.ignoredLines() == 5);
}

TEST_CASE("obj_parser - vertex data") {
  auto ss = std::stringstream(std::string("v -1 1 0\n"
                                          "v -1.0000 0.5000 0.0000\n"
                                          "v 1 0 0\n"
                                          "v 1 1 0\n"));
  Test_OBJ_Parser parser(ss);
  parser.parse();
  REQUIRE(parser.vertex(1) == Point(-1, 1, 0));
  REQUIRE(parser.vertex(2) == Point(-1, 0.5, 0));
  REQUIRE(parser.vertex(3) == Point(1, 0, 0));
  REQUIRE(parser.vertex(4) == Point(1, 1, 0));
}

TEST_CASE("obj_parser - triangle data") {
  auto ss = std::stringstream(std::string("v -1 1 0\n"
                                          "v -1 0 0\n"
                                          "v 1 0 0\n"
                                          "v 1 1 0\n"
                                          "f 1 2 3\n"
                                          "f 1 3 4\n"));
  Test_OBJ_Parser parser(ss);
  parser.parse();

  const auto g = parser.defaultGroup();
  const auto children = g.children;
  const auto t1 = children[0];
  const auto t2 = children[1];

  REQUIRE(t1.p1() == 1);
  REQUIRE(t1.p2() == 2);
  REQUIRE(t1.p3() == 3);
  REQUIRE(t2.p1() == 1);
  REQUIRE(t2.p2() == 3);
  REQUIRE(t2.p3() == 4);
  // REQUIRE(t1.p2 == parser.vertex(2));
  // REQUIRE(t1.p3 == parser.vertex(3));
  // REQUIRE(t2.p1 == parser.vertex(1));
  // REQUIRE(t2.p2 == parser.vertex(3));
  // REQUIRE(t2.p3 == parser.vertex(4));
}

TEST_CASE("obj_parser - polygon data") {
  auto ss = std::stringstream(std::string("v -1 1 0\n"
                                          "v -1 0 0\n"
                                          "v 1 0 0\n"
                                          "v 1 1 0\n"
                                          "v 0 2 0\n"
                                          "f 1 2 3 4 5\n"));
  Test_OBJ_Parser parser(ss);
  parser.parse();

  const auto g = parser.defaultGroup();
  const auto children = g.children;
  const auto t1 = children[0];
  const auto t2 = children[1];
  const auto t3 = children[2];

  REQUIRE(t1.p1() == 1);
  REQUIRE(t1.p2() == 2);
  REQUIRE(t1.p3() == 3);
  REQUIRE(t2.p1() == 1);
  REQUIRE(t2.p2() == 3);
  REQUIRE(t2.p3() == 4);
  REQUIRE(t3.p1() == 1);
  REQUIRE(t3.p2() == 4);
  REQUIRE(t3.p3() == 5);
  // REQUIRE(t1.p1 == parser.vertex(1));
  // REQUIRE(t1.p2 == parser.vertex(2));
  // REQUIRE(t1.p3 == parser.vertex(3));
  // REQUIRE(t2.p1 == parser.vertex(1));
  // REQUIRE(t2.p2 == parser.vertex(3));
  // REQUIRE(t2.p3 == parser.vertex(4));
  // REQUIRE(t3.p1 == parser.vertex(1));
  // REQUIRE(t3.p2 == parser.vertex(4));
  // REQUIRE(t3.p3 == parser.vertex(5));
}

TEST_CASE("obj_parser - named groups") {
  auto ss = std::stringstream(std::string("v -1 1 0\n"
                                          "v -1 0 0\n"
                                          "v 1 0 0\n"
                                          "v 1 1 0\n"
                                          "g FirstGroup\n"
                                          "f 1 2 3\n"
                                          "g SecondGroup\n"
                                          "f 1 3 4\n"));
  Test_OBJ_Parser parser(ss);
  parser.parse();

  auto g1 = parser.getGroup("FirstGroup");
  auto g2 = parser.getGroup("SecondGroup");

  const auto t1 = g1->children[0];
  const auto t2 = g2->children[0];

  REQUIRE(t1.p1() == 1);
  REQUIRE(t1.p2() == 2);
  REQUIRE(t1.p3() == 3);
  REQUIRE(t2.p1() == 1);
  REQUIRE(t2.p2() == 3);
  REQUIRE(t2.p3() == 4);
}

TEST_CASE("obj_parser - convert") {
  auto ss = std::stringstream(std::string("v -1 1 0\n"
                                          "v -1 0 0\n"
                                          "v 1 0 0\n"
                                          "v 1 1 0\n"
                                          "g FirstGroup\n"
                                          "f 1 2 3\n"
                                          "g SecondGroup\n"
                                          "f 1 3 4\n"));
  Test_OBJ_Parser parser(ss);
  parser.parse();

  auto g = parser.createRenderableShapes();
  REQUIRE(g->children().size() == 2);
}

TEST_CASE("obj_parser - normal data") {
  auto ss = std::stringstream(std::string("vn 0 0 1\n"
                                          "vn 0.707 0 -0.707\n"
                                          "vn 1 2 3\n"));
  Test_OBJ_Parser parser(ss);
  parser.parse();
  REQUIRE(parser.normal(1) == Vector(0, 0, 1));
  REQUIRE(parser.normal(2) == Vector(0.707, 0, -0.707));
  REQUIRE(parser.normal(3) == Vector(1, 2, 3));
}

TEST_CASE("obj_parser - faces with normals data") {
  auto ss = std::stringstream(std::string("v 0 1 0\n"
                                          "v -1 0 0\n"
                                          "v 1 0 0\n"
                                          "vn -1 0 0\n"
                                          "vn 1 0 0\n"
                                          "vn 0 1 0\n"
                                          "f 1//3 2//1 3//2\n"
                                          "f 1/0/3 2/102/1 3/14/2\n"));
  Test_OBJ_Parser parser(ss);
  parser.parse();

  const auto g = parser.defaultGroup();
  const auto children = g.children;
  const auto t1 = children[0];
  const auto t2 = children[1];
  REQUIRE(t1.p1() == 1);
  REQUIRE(t1.p2() == 2);
  REQUIRE(t1.p3() == 3);
  REQUIRE(t1.n1() == 3);
  REQUIRE(t1.n2() == 1);
  REQUIRE(t1.n3() == 2);
  REQUIRE(t1 == t2);
}
