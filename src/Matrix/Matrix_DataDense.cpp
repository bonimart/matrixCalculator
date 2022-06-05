#include "Matrix.h"
#include "../settings.h"
#include <memory>

Matrix::DataDense::DataDense(const std::shared_ptr<Data> data,
                             std::size_t shape_y,
                             std::size_t shape_x)
{
    for (std::size_t i = 0; i < shape_y; ++i)
    {
        m_data.push_back({});
        for (std::size_t j = 0; j < shape_x; ++j)
        {
            m_data[i].push_back(data->at(i, j));
        }
    }
}

/**
 * @brief return element at given position
 *
 * @param row
 * @param col
 * @return double value at position [row, col]
 */
double Matrix::DataDense::at(std::size_t row,
                             std::size_t col) const
{
    return m_data.at(row).at(col);
}

/**
 * @brief set value at given position
 *
 * @param row
 * @param col
 * @param val
 */
void Matrix::DataDense::set(std::size_t row,
                            std::size_t col,
                            double val)
{
    m_data[row][col] = val;
}

void Matrix::DataDense::print(std::ostream &out,
                              std::size_t shape_y,
                              std::size_t shape_x) const
{
    out << L_MAT_PAR;
    for (std::size_t i = 0; i < shape_y; ++i)
    {
        if (i != 0)
            out << "," << std::endl
                << " ";
        out << L_MAT_PAR;
        for (std::size_t j = 0; j < shape_x; ++j)
        {
            if (j != 0)
                out << ", ";
            out << this->at(i, j);
        }
        out << R_MAT_PAR;
    }
    out << R_MAT_PAR;
}

std::shared_ptr<Matrix::Data> Matrix::DataDense::clone() const
{
    return std::make_shared<Matrix::DataDense>(m_data);
}
