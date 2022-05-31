#include "Addition.h"

std::unique_ptr<Matrix> Addition::evaluate()
{
    /*
    if (m_left->m_shape.x != m_right->m_shape.x || m_left->m_shape.y != m_right->m_shape.y)
    {
        //! rozdilne rozmery, nejde scitat
    }
    */
    std::unique_ptr<Matrix> m1 = std::move(m_left->evaluate());
    std::unique_ptr<Matrix> m2 = std::move(m_right->evaluate());
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