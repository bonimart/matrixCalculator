#include "Transposition.h"
#include <iostream>

/**
 * @brief transpose given matrix
 *
 * @param p
 * @return std::unique_ptr<Matrix>
 */
std::unique_ptr<Matrix> Transposition::evaluate(Parameters p) const
{
    std::unique_ptr<Matrix> m = std::move(p.param1);
    std::unique_ptr<Matrix> t = std::make_unique<Matrix>(Matrix(m->m_shape_x,
                                                                m->m_shape_y,
                                                                0));
    for (std::size_t i = 0; i < m->m_shape_x; ++i)
    {
        for (std::size_t j = 0; j < m->m_shape_y; ++j)
        {
            t->set(i, j, m->at(j, i));
        }
    }
    return t;
}