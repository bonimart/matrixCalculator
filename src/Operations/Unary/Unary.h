#pragma once

#include "../Operation.h"

class Unary : public Operation
{
public:
    virtual void validate(const Parameters &p) const override
    {
        if (p.paramCount != numOfOperands())
            throw std::runtime_error("Invalid number of arguments for unary function");
    }
    virtual int numOfOperands() const override
    {
        return 1;
    }
};