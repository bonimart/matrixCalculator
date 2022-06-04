#pragma once

#include "../Operation.h"

class Binary : public Operation
{
public:
    virtual void validate(const Parameters &p) const override
    {
        if (p.paramCount != numOfOperands())
            throw std::runtime_error("Invalid number of arguments for binary function");
    }
    virtual int numOfOperands() const override
    {
        return 2;
    }
};