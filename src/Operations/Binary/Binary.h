#pragma once

#include "../Operation.h"
//#include <memory>

class Binary : public Operation
{
protected:
    std::unique_ptr<Matrix> m_left;
    std::unique_ptr<Matrix> m_right;
};