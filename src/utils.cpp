#include "utils.h"
#include <limits>
#include <memory>
#include <iostream>

/**
 * @brief double comparison
 * 
 * @param a double
 * @param b double
 * @return true if values match
 * @return false otherwise
 */
bool doubleCmp(const double a, const double b)
{
    return a == b ||
           std::abs(a - b) < std::abs(std::min(a, b)) * std::numeric_limits<double>::epsilon();
}

bool isValidFileName(const char c)
{
    return std::isalpha(c) || c == '.' || c == '/';
}