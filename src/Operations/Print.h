#pragma once

#include <iostream>
#include "Operation.h"

class Print : public Operation
{
protected:
    std::istream is;
    std::unique_ptr<Matrix> param;

public:
    //virtual std::unique_ptr<Matrix> evaluate() override;
};