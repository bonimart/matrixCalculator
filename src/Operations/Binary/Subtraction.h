#pragma once

#include "Binary.h"

/**
 * @brief class for matrix subtraction
 *
 */
class Subtraction : public Binary
{
public:
    virtual void validate(const Parameters &p) const override;
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override;
};