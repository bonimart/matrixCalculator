#pragma once

#include "Unary.h"
//#include <memory>

class Transposition : public Unary
{
    virtual std::unique_ptr<Matrix> evaluate() override;
};