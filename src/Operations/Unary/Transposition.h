#pragma once

#include "Unary.h"

class Transposition : public Unary
{
public:
    Transposition(std::unique_ptr<Operation> op)
        : Unary(std::move(op)) {}
    virtual std::unique_ptr<Matrix> evaluate() override;
};