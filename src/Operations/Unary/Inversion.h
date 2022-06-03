#pragma once

#include "Unary.h"

class Inversion : public Unary
{
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override;
};