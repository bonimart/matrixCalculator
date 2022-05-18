#include "Operations/Binary/Selection.h"
#include "Matrix/MatrixDense.h"

std::unique_ptr<Matrix> Selection::evaluate()
{
    int x0 = m_right->get({0, 0});
    int x1 = m_right->get({1, 0});
    int y0 = m_right->get({0, 1});
    int y1 = m_right->get({1, 1});

    int min_x = x0 < x1 ? x0 : x1;
    int max_x = x0 >= x1 ? x0 : x1;
    int min_y = y0 < y1 ? y0 : y1;
    int max_y = y0 >= y1 ? y0 : y1;

    if (m_left->m_shape.x < max_x || m_left->m_shape.y < max_y || min_x < 0 || min_y < 0)
    {
        //! nespravne rozmery pro vyber
    }
    std::unique_ptr<Matrix> m = std::make_unique<MatrixDense>(Index{max_x - min_x, max_y - min_y}, 0);

    for (int i = min_x; i < max_x; i++)
    {
        for (int j = min_y; j < max_y; j++)
        {
            m->set({i - min_x, j - min_y}, m_left->get({i, j}));
        }
    }

    return m;
}