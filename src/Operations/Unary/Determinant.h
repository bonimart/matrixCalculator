#pragma once

#include "Unary.h"
//#include <memory>

class Determinant : public Unary
{
    virtual std::unique_ptr<Matrix> evaluate() override;
};