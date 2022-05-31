#pragma once
#include <cstddef>

class Index
{
private:
public:
    Index(const std::size_t x_coord, const std::size_t y_coord)
        : x(x_coord), y(y_coord) {}

    std::size_t x;
    std::size_t y;
};