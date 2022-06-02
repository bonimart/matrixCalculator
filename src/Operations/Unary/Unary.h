#pragma once

#include "../Operation.h"
//#include <memory>

class Unary : public Operation
{
public:
    virtual bool validate(const Parameters &p) const override { return true; }
    virtual int numOfOperands() const override
    {
        return 1;
    }
};