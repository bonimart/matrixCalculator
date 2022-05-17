#pragma once

#include "Matrix.h"
#include <vector>

class MatrixDense : public Matrix
{
protected:
    std::vector<std::vector<double>> m_data;

public:
    MatrixDense(const std::vector<std::vector<double>> &data);

    virtual double &get(const Index &coords) const override;
};