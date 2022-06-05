#pragma once

#include "Unary.h"

/**
 * @brief class for matrix transposition
 *
 */
class Transposition : public Unary
{
public:
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override;
};