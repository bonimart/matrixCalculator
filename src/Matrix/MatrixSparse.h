#pragma once

#include "Matrix.h"
#include <unordered_map>
#include <vector>

class MatrixSparse : public Matrix
{
protected:
    std::unordered_map<int, std::unordered_map<int, double>> m_data;

public:
    MatrixSparse(const std::vector<std::vector<double>> &data);

    virtual double get(const Index &coords) const override;
    virtual void set(const Index &coords, const double &val) override;
};