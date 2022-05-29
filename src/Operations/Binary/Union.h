#pragma once

#include "Binary.h"
//#include "Matrix/Matrix.h"

class Union : public Binary
{
    virtual std::unique_ptr<Matrix> evaluate() override;
};