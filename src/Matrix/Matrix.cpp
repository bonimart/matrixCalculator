#include "Matrix.h"
#include "../settings.h"
#include <limits>

bool Matrix::doubleCmp(const double a, const double b)
{
    return a == b ||
           std::abs(a - b) < std::abs(std::min(a, b)) * std::numeric_limits<double>::epsilon();
}

Matrix::Matrix(const std::vector<std::vector<double>> &data)
    : m_shape_y(data.size()), m_shape_x(data.at(0).size())
{
    for (std::size_t i = 0; i < m_shape_y; ++i)
    {
        for (std::size_t j = 0; j < m_shape_x; ++j)
        {
            if (doubleCmp(data.at(i).at(j), 0))
                ++zeroCount;
        }
    }
    if (zeroCount < SPARSE_THRESHOLD * m_shape_x * m_shape_y)
    {
        m_data = std::make_shared<DataDense>(data);
    }
    else
    {
        m_data = std::make_shared<DataSparse>(data);
    }
}

Matrix::Matrix(const std::size_t shape_y, const std::size_t shape_x, const double fill)
    : m_shape_y(shape_y),
      m_shape_x(shape_x)
{
    if (doubleCmp(fill, 0))
        zeroCount = m_shape_x * m_shape_y;
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
    if (zeroCount < SPARSE_THRESHOLD * m_shape_x * m_shape_y)
    {
        m_data = std::make_shared<DataDense>(data);
    }
    else
    {
        m_data = std::make_shared<DataSparse>(data);
    }
}

double Matrix::at(std::size_t i, std::size_t j) const
{
    return m_data->at(i, j);
}

void Matrix::set(std::size_t i, std::size_t j, double val)
{
    lastZeroCount = zeroCount;
    zeroCount += doubleCmp(val, 0) - doubleCmp(at(i, j), 0);
    m_data->set(i, j, val);
    balance();
}

/**
 * @brief Balance data in order to achieve effective memory management.
 * When the matrix is sparse, save the data as map, otherwise save it as a two dimensional array.
 *
 */
void Matrix::balance()
{
    bool wasBelowThreshold = lastZeroCount < SPARSE_THRESHOLD * m_shape_x * m_shape_y;
    bool isBelowThreshold = zeroCount < SPARSE_THRESHOLD * m_shape_x * m_shape_y;
    if (wasBelowThreshold == isBelowThreshold)
        return;
    else if (isBelowThreshold)
    {
        m_data = std::make_shared<DataDense>(m_data, m_shape_y, m_shape_x);
    }
    else
    {
        m_data = std::make_shared<DataSparse>(m_data, m_shape_y, m_shape_x);
    }
}

void Matrix::print(std::ostream &out) const
{
    m_data->print(out, m_shape_y, m_shape_x);
}
