#include "Subtraction.h"
#include "Addition.h"

void Subtraction::validate(const Parameters &p) const
{
    if (p.param1->m_shape_x != p.param2->m_shape_x ||
        p.param1->m_shape_y != p.param2->m_shape_y)
    {
        throw std::runtime_error("Matrix subtraction is defined for matries of the same shape only.");
    }
}

std::unique_ptr<Matrix> Subtraction::evaluate(Parameters p) const
{
    validate(p);
    Addition a;
    *p.param2 = *p.param2 * -1;
    return a.evaluate(std::move(p));
}