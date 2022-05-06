#pragma once

#include "Matrix.h"
#include <unordered_map>

class SparseMatrix : public Matrix
{
protected:
    std::unordered_map<Point, double> m_data;

public:
    SparseMatrix(const std::unordered_map<Point, double> &data);

    virtual Matrix &operator=(const Matrix &other);
    virtual Matrix &transpose() const override;
    virtual Matrix &crop(const Point &shape) const override;

    virtual void print(std::ostream &os) const override;
    virtual double &get(const Point &coords) const override;
};