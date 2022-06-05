#pragma once

#include "Unary.h"

/**
 * @brief Class for matrix rank calculation
 *
 */
class Rank : public Unary
{
public:
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override;
};