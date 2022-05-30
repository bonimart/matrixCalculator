#pragma once

#include "Operation.h"

class Identity : public Operation
{
public:
    virtual std::unique_ptr<Matrix> evaluate() override;
};