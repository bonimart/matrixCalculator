#pragma once

#include "Matrix.h"
#include <unordered_map>

class MatrixSparse : public Matrix
{
protected:
    std::unordered_map<Index, double> m_data;

public:
    MatrixSparse(const std::unordered_map<Index, double> &data);

    virtual double &get(const Index &coords) const override;
};