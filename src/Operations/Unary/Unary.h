#pragma once

#include "../Operation.h"
//#include <memory>

class Unary : public Operation
{
public:
    virtual void validate(const Parameters &p) const override {}
    virtual int numOfOperands() const override
    {
        return 1;
    }
};