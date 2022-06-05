#include "Union.h"

void Union::validate(const Parameters &p) const
{
    if (p.param1->m_shape_y != p.param2->m_shape_y)
    {
        throw std::runtime_error("Union takes two matrices with same number of rows");
    }
}

std::unique_ptr<Matrix> Union::evaluate(Parameters p) const
{
    //! musi sedet pocet radku
    std::unique_ptr<Matrix> m = std::make_unique<Matrix>(p.param1->m_shape_y,
                                                         p.param1->m_shape_x + p.param2->m_shape_x);
    for (std::size_t i = 0; i < m->m_shape_y; ++i)
    {
        for (std::size_t j = 0; j < p.param1->m_shape_x; ++j)
        {
            m->set(i, j, p.param1->at(i, j));
        }
        for (std::size_t j = 0; j < p.param2->m_shape_x; ++j)
        {
            m->set(i, j + p.param1->m_shape_x, p.param2->at(i, j));
        }
    }
    return m;
}