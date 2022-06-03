#pragma once

#include "Unary.h"
//#include <memory>

class GEM : public Unary
{
public:
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override;
};