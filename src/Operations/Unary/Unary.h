#pragma once

#include "../Operation.h"
//#include <memory>

class Unary : public Operation
{
protected:
    /*
        std::unique_ptr<Operation> m_param;
        Unary(std::unique_ptr<Operation> op)
            : m_param(std::move(op)) {}
    */
    virtual int numOfOperands() override
    {
        return 1;
    }
};