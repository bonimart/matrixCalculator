#include "utils.h"
#include <limits>
#include <memory>
#include <iostream>

bool doubleCmp(const double a, const double b)
{
    return a == b ||
           std::abs(a - b) < std::abs(std::min(a, b)) * std::numeric_limits<double>::epsilon();
}