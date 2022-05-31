#pragma once

#include <vector>
#include <iostream>
#include "../Index.h"

class Matrix
{
protected:
    std::vector<std::vector<double>> m_data;
    size_t m_shape_x;
    size_t m_shape_y;

public:
    Matrix(const std::vector<std::vector<double>> &data)
        : m_data(data), m_shape_y(data.size()), m_shape_x(data[0].size()) {}
    Matrix(const Matrix &other)
        : m_data(other.m_data), m_shape_x(other.m_shape_x), m_shape_y(other.m_shape_y) {}
    ~Matrix() = default;

    Matrix &operator=(const Matrix &other);

    bool validIndex(const Index &coords) const;

    double get(const Index &coords) const;

    void print(std::ostream &out) const;
    // virtual double get(const Index &coords) const = 0;
    // virtual void set(const Index &coords, const double &val) = 0;
};