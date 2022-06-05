#pragma once

#include "Unary.h"

class Inversion : public Unary
{
public:
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override;
    virtual void validate(const Parameters &p) const override;
};