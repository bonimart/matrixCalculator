#include "Multiplication.h"
#include "../../Matrix/MatrixDense.h"

/**
std::unique_ptr<Matrix> Multiplication::evaluate()
{
    if (m_left->m_shape.y != m_right->m_shape.x)
    {
        //! rozdilne rozmery, nejde scitat
    }
    std::unique_ptr<Matrix> m = std::make_unique<MatrixDense>(Index{m_left->m_shape.x, m_right->m_shape.y}, 0);
    for (int i = 0; i < m_left->m_shape.x; i++)
    {
        for (int j = 0; j < m_right->m_shape.y; j++)
        {
            double sum = 0;
            for (int k = 0; k < m_left->m_shape.y; k++)
            {
                sum += m_left->get({i, k}) * m_right->get({k, j});
            }
            m->set({i, j}, sum);
        }
    }
    return m;
}
**/