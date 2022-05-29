#pragma once

#include <vector>
#include "../Index.h"

class Matrix
{
public:
    Index m_shape;

    Matrix();
    // Matrix(const std::vector<std::vector<double>> &data);
    Matrix(const Matrix &other);
    virtual ~Matrix() = default;

    Matrix &operator=(const Matrix &other);

    bool validIndex(const Index &coords) const;

    virtual double get(const Index &coords) const = 0;
    virtual void set(const Index &coords, const double &val) = 0;
};