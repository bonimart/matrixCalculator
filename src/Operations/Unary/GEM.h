#pragma once

#include "Unary.h"

/**
 * @brief Class for gaussian elimination
 *
 */
class GEM : public Unary
{
public:
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override;
};