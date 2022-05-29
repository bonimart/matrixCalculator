#pragma once

class Index
{
private:
public:
    Index(const int x_coord, const int y_coord)
        : x(x_coord), y(y_coord) {}

    int x;
    int y;
};