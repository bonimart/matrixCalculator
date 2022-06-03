#include "utils.h"
#include <limits>
#include <memory>
#include <iostream>

/**
 * @brief xdd
 *
 * @param a
 * @param b
 * @return true
 * @return false
 */
bool doubleCmp(const double a, const double b)
{
    return a == b ||
           std::abs(a - b) < std::abs(std::min(a, b)) * std::numeric_limits<double>::epsilon();
}