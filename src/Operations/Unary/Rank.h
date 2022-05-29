#pragma once

#include "Unary.h"
//#include <memory>

class Rank : public Unary
{
    virtual std::unique_ptr<Matrix> evaluate() override;
};