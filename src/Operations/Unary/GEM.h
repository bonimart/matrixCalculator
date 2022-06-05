#pragma once

#include "Unary.h"

class GEM : public Unary
{
public:
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override;
};