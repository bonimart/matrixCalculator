#include "Addition.h"

void Addition::validate(const Parameters &p) const
{
    if (p.param1->m_shape_x != p.param2->m_shape_x ||
        p.param1->m_shape_y != p.param2->m_shape_y)
    {
        throw std::runtime_error("Matrix addition is defined for matries of the same shape only.");
    }
}

std::unique_ptr<Matrix> Addition::evaluate(Parameters p) const
{
    validate(p);
    std::unique_ptr<Matrix> m1 = std::move(p.param1);
    std::unique_ptr<Matrix> m2 = std::move(p.param2);
    std::unique_ptr<Matrix> res = std::make_unique<Matrix>(m1->m_shape_y, m1->m_shape_x, 0);
    for (std::size_t i = 0; i < m1->m_shape_y; ++i)
    {
        for (std::size_t j = 0; j < m1->m_shape_x; ++j)
        {
            res->set(i, j, m1->at(i, j) + m2->at(i, j));
        }
    }
    return res;
}