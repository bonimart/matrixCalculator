#pragma once

#include "Unary.h"

class Rank : public Unary
{
public:
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override;
};