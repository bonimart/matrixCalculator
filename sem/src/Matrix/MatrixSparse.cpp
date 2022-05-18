#include "Matrix/MatrixSparse.h"

double MatrixSparse::get(const Index &coords) const
{
    if (!validIndex(coords))
    {
        //! chyba
    }
    if (m_data.count(coords.x) == 0 || m_data.at(coords.x).count(coords.y) == 0)
    {
        return 0;
    }
    return m_data.at(coords.x).at(coords.y);
}

void MatrixSparse::set(const Index &coords, const double &val)
{
    if (!validIndex(coords))
    {
        //! chyba
    }
    m_data[coords.x][coords.y] = val;
}