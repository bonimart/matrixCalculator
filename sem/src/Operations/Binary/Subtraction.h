#pragma once

#include "Operations/Binary/Binary.h"
//#include "Matrix/Matrix.h"

class Subtraction : public Binary
{
    virtual std::unique_ptr<Matrix> evaluate() override;
};