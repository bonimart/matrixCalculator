#pragma once

#include "../Matrix/Matrix.h"
#include "Parameters/Parameter.h"
#include <memory>

class Operation
{
public:
    virtual ~Operation() = default;
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const = 0;
    virtual bool validate(const Parameters &p) const = 0;
    virtual int numOfOperands() const = 0;
};