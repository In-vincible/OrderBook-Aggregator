#ifndef DOUBLE_UTILS_H
#define DOUBLE_UTILS_H

#include <cmath>

const double TOLERANCE = 1e-9;

bool doubleEquals(double a, double b, double tolerance = TOLERANCE) {
    return std::fabs(a - b) <= tolerance;
}

#endif