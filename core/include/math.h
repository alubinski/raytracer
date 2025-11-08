#pragma once
#include <cmath>
#include <cstdlib>

const float epsilon = 1e-4;
const float SQRT_2 = std::sqrt(2);

inline bool epsilonEqual(float a, float b) { return std::abs(a - b) < epsilon; }
