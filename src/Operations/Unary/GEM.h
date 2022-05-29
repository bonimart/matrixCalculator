#pragma once

#include "Unary.h"
//#include <memory>

class GEM : public Unary
{
    virtual std::unique_ptr<Matrix> evaluate() override;
};