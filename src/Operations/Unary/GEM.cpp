#include "GEM.h"
#include "../../utils.h"

std::unique_ptr<Matrix> GEM::evaluate(Parameters p) const
{
    std::unique_ptr<Matrix> m = std::move(p.param1);
    for (std::size_t i = 0; i < std::min(m->m_shape_x, m->m_shape_y); ++i)
    {
        //*dokud je 0 na pozici [i,j], swapuj radky, pokud jsou samy nuly, jdi dal
        for (std::size_t row = i + 1;
             row < m->m_shape_y && doubleCmp(m->at(i, i), 0);
             ++row)
        {

            if (!doubleCmp(m->at(row, i), 0))
            {
                double tmp;
                for (std::size_t col = 0; col < m->m_shape_x; ++col)
                {
                    tmp = m->at(i, col);
                    m->set(i, col, m->at(row, col));
                    m->set(row, col, tmp);
                }
            }
        }

        //*od vsech radku pod i odecti nasobek radku i
        for (std::size_t row = i + 1;
             row < m->m_shape_y && !doubleCmp(m->at(i, i), 0);
             ++row)
        {
            double coefficient = m->at(row, i) / m->at(i, i);
            for (std::size_t col = 0; col < m->m_shape_x; ++col)
            {
                m->set(row,
                       col,
                       m->at(row, col) - m->at(i, col) * coefficient);
            }
        }
    }
    //*zpetny chod
    for (int i = m->m_shape_y - 1; i >= 0; --i)
    {
        for (std::size_t j = 0; j < m->m_shape_x; ++j)
        {
            if (doubleCmp(m->at(i, j), 0))
                continue;
            //? normalize first non-zero value to 1, update rest of the row accordingly
            double coef = m->at(i, j);
            for (std::size_t col = 0; col < m->m_shape_x; ++col)
            {
                m->set(i, col, m->at(i, col) / coef);
            }
            for (int row = i - 1; row >= 0; --row)
            {
                double coefficient = m->at(row, j);
                for (std::size_t col = 0; col < m->m_shape_x; ++col)
                {
                    m->set(row,
                           col,
                           m->at(row, col) - m->at(i, col) * coefficient);
                }
            }
            break;
        }
    }
    return m;
}