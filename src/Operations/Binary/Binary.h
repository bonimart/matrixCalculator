#pragma once

#include "../Operation.h"

class Binary : public Operation
{
protected:
    std::unique_ptr<Operation> m_left;
    std::unique_ptr<Operation> m_right;
    virtual int numOfOperands() override
    {
        return 2;
    }
};