#pragma once

#include "Binary.h"

/**
 * @brief Class for matrix addition
 *
 */
class Addition : public Binary
{
public:
    virtual void validate(const Parameters &p) const override;
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override;
};