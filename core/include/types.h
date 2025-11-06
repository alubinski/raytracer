#pragma once
#include <memory>

class Shape;
using ShapePtr = std::shared_ptr<Shape>;
using ShapeConstPtr = std::shared_ptr<const Shape>;

class Pattern;
using PatternPtr = std::shared_ptr<Pattern>;
using PatternConstPtr = std::shared_ptr<const Pattern>;
