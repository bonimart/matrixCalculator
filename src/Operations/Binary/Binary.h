#pragma once

#include "../Operation.h"

class Binary : public Operation
{
public:
    virtual void validate(const Parameters &p) const override {}
    virtual int numOfOperands() const override
    {
        return 2;
    }
};