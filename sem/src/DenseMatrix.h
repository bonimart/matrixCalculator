#pragma once

#include "Matrix.h"
#include <vector>

class DenseMatrix : public Matrix
{
protected:
    std::vector<std::vector<double>> m_data;

public:
    DenseMatrix(const std::vector<std::vector<double>> &data);

    virtual Matrix &operator=(const Matrix &other);
    virtual Matrix &transpose() const override;
    virtual Matrix &crop(const Point &shape) const override;

    virtual void print(std::ostream &os) const override;
    virtual double &get(const Point &coords) const override;
};