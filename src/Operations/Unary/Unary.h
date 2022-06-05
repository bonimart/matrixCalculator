#pragma once

#include "../Operation.h"

/**
 * @brief a class that puts all unary operations together
 *
 */
class Unary : public Operation
{
public:
    virtual void validate(const Parameters &p) const override
    {
        if (p.paramCount != numOfOperands())
            throw std::runtime_error("Invalid number of arguments for unary function");
    }
    virtual int numOfOperands() const override
    {
        return 1;
    }
};