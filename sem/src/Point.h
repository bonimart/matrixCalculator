#pragma once

class Point
{
private:
    static bool isValid(int x, int y);

public:
    Point(const int x_coord, const int y_coord);

    int x;
    int y;
};