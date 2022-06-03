#include "Rank.h"
#include "GEM.h"
#include "../../utils.h"

std::unique_ptr<Matrix> Rank::evaluate(Parameters p) const
{
    GEM g;
    std::unique_ptr<Matrix> m = g.evaluate(std::move(p.param1));
    int emptyRows = 0;
    for (int i = m->m_shape_y - 1; i >= 0; --i)
    {
        bool empty = true;
        for (std::size_t j = 0; j < m->m_shape_x; ++j)
        {
            if (!doubleCmp(m->at(i, j), 0))
            {
                empty = false;
                break;
            }
        }
        if (empty)
            emptyRows++;
    }
    int rank = m->m_shape_y - emptyRows < m->m_shape_x ? m->m_shape_y - emptyRows : m->m_shape_x;

    return std::make_unique<Matrix>(1, 1, rank);
}