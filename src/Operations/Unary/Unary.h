#pragma once

#include "../Operation.h"
//#include <memory>

class Unary : public Operation
{
protected:
    std::unique_ptr<Matrix> m_param;
};