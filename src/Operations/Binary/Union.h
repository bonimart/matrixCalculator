#pragma once

#include "Binary.h"

/**
 * @brief class that can unite matrices by joining their columns
 *
 */
class Union : public Binary
{
public:
    virtual void validate(const Parameters &p) const override;
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override;
};