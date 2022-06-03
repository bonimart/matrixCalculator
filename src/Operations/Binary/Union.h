#pragma once

#include "Binary.h"

class Union : public Binary
{
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override;
};