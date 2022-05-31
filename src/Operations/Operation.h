#pragma once

#include "../Matrix/Matrix.h"
#include "Parameters/Parameter.h"
#include <memory>

class Operation
{
public:
    virtual ~Operation() = default;
    // virtual std::unique_ptr<Matrix> evaluate() = 0;
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) = 0;
    virtual int numOfOperands() = 0;
};