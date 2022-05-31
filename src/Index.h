#pragma once

class Index
{
private:
public:
    Index(const size_t x_coord, const size_t y_coord)
        : x(x_coord), y(y_coord) {}

    size_t x;
    size_t y;
};