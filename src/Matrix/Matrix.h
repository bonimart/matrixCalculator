#pragma once

#include <vector>
#include <iostream>
#include "../Index.h"

class Matrix
{
protected:
    std::vector<std::vector<double>> m_data;

public:
    std::size_t m_shape_y = 0;
    std::size_t m_shape_x = 0;
    Matrix(const std::vector<std::vector<double>> &data)
        : m_data(data), m_shape_y(data.size()), m_shape_x(data[0].size()) {}
    Matrix(const Matrix &other)
        : m_data(other.m_data), m_shape_y(other.m_shape_y), m_shape_x(other.m_shape_x) {}
    Matrix(const std::size_t shape_y, const std::size_t shape_x, const double fill);
    ~Matrix() = default;

    Matrix &operator=(const Matrix &other);

    bool validIndex(const Index &coords) const;

    double get(const Index &coords) const;
    double at(std::size_t i, std::size_t j) const;
    void set(std::size_t i, std::size_t j, double val);

    void print(std::ostream &out) const;
    // virtual double get(const Index &coords) const = 0;
    // virtual void set(const Index &coords, const double &val) = 0;
};