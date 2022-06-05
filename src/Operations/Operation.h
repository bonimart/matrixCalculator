#pragma once

#include "../Matrix/Matrix.h"
#include "Parameters/Parameter.h"
#include <memory>

class Operation
{
public:
    Operation() = default;
    virtual ~Operation() = default;
    Operation(const Operation &other) = delete;

    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const = 0;
    virtual void validate(const Parameters &p) const = 0;
    virtual int numOfOperands() const = 0;
};