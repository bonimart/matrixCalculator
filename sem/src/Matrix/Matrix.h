#pragma once

#include <iostream>
#include "Index.h"

class Matrix
{
public:
    Index m_shape;

    Matrix(const int x, const int y);
    Matrix(const Matrix &other);
    virtual ~Matrix() = default;

    Matrix &operator=(const Matrix &other);

    virtual double &get(const Index &coords) const = 0;
};