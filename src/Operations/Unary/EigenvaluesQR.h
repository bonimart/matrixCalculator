#pragma once

#include "Unary.h"
//#include <memory>

class EigenvaluesQR : public Unary
{
    virtual std::unique_ptr<Matrix> evaluate() override;
};