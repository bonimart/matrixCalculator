#pragma once

#include "Binary.h"

class Selection : public Binary
{
public:
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override;
};