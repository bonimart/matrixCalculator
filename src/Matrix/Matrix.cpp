#include "Matrix.h"
#include "../settings.h"

bool Matrix::validIndex(const Index &coords) const
{
    return coords.x >= 0 && coords.x < m_shape_x && coords.y >= 0 && coords.y < m_shape_y;
}

double Matrix::get(const Index &coords) const
{
    return m_data.at(coords.x).at(coords.y);
}

void Matrix::print(std::ostream &out) const
{
    out << L_MAT_PAR;
    for (size_t i = 0; i < m_shape_y; ++i)
    {
        if (i != 0)
            out << "," << std::endl
                << " ";
        out << L_MAT_PAR;
        for (size_t j = 0; j < m_shape_x; ++j)
        {
            if (j != 0)
                out << ", ";
            out << this->get({i, j});
        }
        out << R_MAT_PAR;
    }
    out << R_MAT_PAR;
}