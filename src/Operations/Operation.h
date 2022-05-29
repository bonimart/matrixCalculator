#pragma once

#include "../Matrix/Matrix.h"
#include <memory>

class Operation
{
protected:
public:
    virtual ~Operation();
    virtual std::unique_ptr<Matrix> evaluate() = 0;
};