#pragma once

#include "Operation.h"

class Identity : public Operation
{
public:
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) override
    {
        return std::move(p.param1);
    }
    virtual int numOfOperands() override
    {
        return 1;
    }
};