#pragma once

#include "Unary.h"
//#include <memory>

class Inversion : public Unary
{
    virtual std::unique_ptr<Matrix> evaluate() override;
};