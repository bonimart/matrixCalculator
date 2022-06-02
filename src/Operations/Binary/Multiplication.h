#pragma once

#include "Binary.h"
//#include "Matrix/Matrix.h"

class Multiplication : public Binary
{
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override;
};