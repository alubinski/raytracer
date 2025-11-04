#pragma once
#include <cstdlib>

const float epsilon = 1e-4;

inline bool epsilonEqual(float a, float b) { return std::abs(a - b) < epsilon; }
