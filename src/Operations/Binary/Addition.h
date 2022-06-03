#pragma once

#include "Binary.h"
//#include "Matrix/Matrix.h"<

class Addition : public Binary
{
public:
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override;
};