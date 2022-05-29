#include "Addition.h"
#include "../../Matrix/MatrixDense.h"

/**
std::unique_ptr<Matrix> Addition::evaluate()
{
    if (m_left->m_shape.x != m_right->m_shape.x || m_left->m_shape.y != m_right->m_shape.y)
    {
        //! rozdilne rozmery, nejde scitat
    }
    std::unique_ptr<Matrix> m = std::make_unique<MatrixDense>(Index{m_left->m_shape.x, m_left->m_shape.y}, 0);
    for (int i = 0; i < m_left->m_shape.x; i++)
    {
        for (int j = 0; j < m_left->m_shape.y; j++)
        {
            m->set({i, j}, m_left->get({i, j}) + m_right->get({i, j}));
        }
    }
    return m;
}
**/