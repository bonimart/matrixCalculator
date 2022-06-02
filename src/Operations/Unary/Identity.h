#pragma once

#include "Unary.h"

class Identity : public Unary
{
public:
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override
    {
        return std::move(p.param1);
    }
};