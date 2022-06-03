#include "Union.h"

/*
std::unique_ptr<Matrix> Union::evaluate()
{
    if (m_left->m_shape.y != m_right->m_shape.y)
    {
        //! rozdilne rozmery, nende
    }
    std::unique_ptr<Matrix> m = std::make_unique<MatrixDense>(Index{m_left->m_shape.x + m_right->m_shape.x, m_left->m_shape.y}, 0);
    for (int j = 0; j < m_left->m_shape.y; j++)
    {
        for (int i = 0; i < m_left->m_shape.x; i++)
        {
            m->set({i, j}, m_left->get({i, j}));
        }
        for (int i = 0; i < m_right->m_shape.x; i++)
        {
            m->set({m_left->m_shape.x + i, j}, m_right->get({i, j}));
        }
    }

    return m;
}
*/