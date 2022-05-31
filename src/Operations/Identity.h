#pragma once

#include "Operation.h"

class Identity : public Operation
{
protected:
    Matrix m_matrix;

public:
    Identity(const Matrix &m)
        : m_matrix(m) {}
    virtual ~Identity() {}
    virtual std::unique_ptr<Matrix> evaluate() override
    {
        return std::make_unique<Matrix>(m_matrix);
    }
    virtual int numOfOperands() override
    {
        return 1;
    }
};