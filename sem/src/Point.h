#pragma once

class Point
{
private:
    bool isValid(int x, int y);

public:
    Point(const int x_coord, const int y_coord);

    int x;
    int y;
};