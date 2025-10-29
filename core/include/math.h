#pragma once
#include <cstdlib>

const float epsilon = 1e-5;

inline bool epsilonEqual(float a, float b) { return abs(a - b) < epsilon; }
