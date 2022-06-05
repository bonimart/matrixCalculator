#pragma once

#include "Binary.h"

/**
 * @brief Class for selecting part of a matrix
 *
 */
class Selection : public Binary
{
public:
    virtual void validate(const Parameters &p) const override;
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override;
};