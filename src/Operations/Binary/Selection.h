#pragma once

#include "Binary.h"

class Selection : public Binary
{
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override;
};