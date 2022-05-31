#include "Matrix.h"
#include "../settings.h"

Matrix::Matrix(const std::size_t shape_y, const std::size_t shape_x, const double fill)
    : m_shape_y(shape_y), m_shape_x(shape_x)
{
    std::vector<std::vector<double>> data;
    std::vector<double> vec_x;
    for (std::size_t i = 0; i < m_shape_y; ++i)
    {
        data.push_back({});
        for (std::size_t j = 0; j < m_shape_x; ++j)
        {
            data.at(i).push_back(fill);
        }
    }
    m_data = data;
}

bool Matrix::validIndex(const Index &coords) const
{
    return coords.x < m_shape_x && coords.y < m_shape_y;
}

double Matrix::get(const Index &coords) const
{
    return m_data.at(coords.x).at(coords.y);
}

double Matrix::at(std::size_t i, std::size_t j) const
{
    return m_data.at(i).at(j);
}

void Matrix::set(std::size_t i, std::size_t j, double val)
{
    m_data[i][j] = val;
}

void Matrix::print(std::ostream &out) const
{
    out << L_MAT_PAR;
    for (std::size_t i = 0; i < m_shape_y; ++i)
    {
        if (i != 0)
            out << "," << std::endl
                << " ";
        out << L_MAT_PAR;
        for (std::size_t j = 0; j < m_shape_x; ++j)
        {
            if (j != 0)
                out << ", ";
            out << this->get({i, j});
        }
        out << R_MAT_PAR;
    }
    out << R_MAT_PAR;
}