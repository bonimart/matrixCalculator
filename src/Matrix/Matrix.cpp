#include "Matrix.h"
#include "../settings.h"
#include "../utils.h"

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

Matrix::Matrix(const std::unordered_map<std::size_t, std::unordered_map<std::size_t, double>> &data)
{
    int nonZeros = 0;
    std::size_t rows = 0;
    std::size_t columns = 0;
    for (auto &[row, cols] : data)
    {
        if (row + 1 > rows)
            rows = row + 1;
        for (auto &[col, val] : cols)
        {
            if (col + 1 > columns)
                columns = col + 1;
            if (!doubleCmp(val, 0))
                nonZeros++;
        }
    }
    m_shape_y = rows;
    m_shape_x = columns;
    zeroCount = rows * columns - nonZeros;
    if (zeroCount < SPARSE_THRESHOLD * m_shape_x * m_shape_y)
    {
        m_data = std::make_shared<DataDense>(data);
    }
    else
    {
        m_data = std::make_shared<DataSparse>(data);
    }
}

Matrix::Matrix(const std::size_t rows,
               const std::size_t columns,
               const double fill)
    : m_shape_y(rows),
      m_shape_x(columns)
{
    if (doubleCmp(fill, 0))
    {
        zeroCount = m_shape_x * m_shape_y;
        m_data = std::make_shared<DataSparse>();
        return;
    }

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
    m_data = std::make_shared<DataDense>(data);
}

Matrix::Matrix(const std::size_t &rows,
               const std::size_t &columns)
{
    *this = Matrix(rows, columns, 0);
    for (std::size_t i = 0; i < rows; ++i)
    {
        set(i, i, 1);
    }
}

Matrix::Matrix(const Matrix &other)
{
    m_shape_y = other.m_shape_y;
    m_shape_x = other.m_shape_x;
    zeroCount = other.zeroCount;
    m_data = other.m_data->clone();
}

Matrix &Matrix::operator=(const Matrix &other)
{
    m_shape_y = other.m_shape_y;
    m_shape_x = other.m_shape_x;
    zeroCount = other.zeroCount;
    m_data = other.m_data->clone();
    return *this;
}

Matrix Matrix::operator+(const double val) const
{
    Matrix m = Matrix(m_shape_y, m_shape_x, 0);
    for (std::size_t i = 0; i < m_shape_y; ++i)
    {
        for (std::size_t j = 0; j < m_shape_x; ++j)
        {
            m.set(i, j, this->at(i, j) + val);
        }
    }
    return m;
}
Matrix Matrix::operator*(const double val) const
{
    Matrix m = Matrix(m_shape_y, m_shape_x, 0);
    for (std::size_t i = 0; i < m_shape_y; ++i)
    {
        for (std::size_t j = 0; j < m_shape_x; ++j)
        {
            m.set(i, j, this->at(i, j) * val);
        }
    }
    return m;
}

/**
 * @brief return element at given position
 *
 * @param row
 * @param col
 * @return double value at position [row, col]
 */
double Matrix::at(std::size_t row,
                  std::size_t col) const
{
    return m_data->at(row, col);
}

/**
 * @brief set value at given position
 *
 * @param row
 * @param col
 * @param val
 */
void Matrix::set(std::size_t row,
                 std::size_t col,
                 double val)
{
    lastZeroCount = zeroCount;
    zeroCount += doubleCmp(val, 0) - doubleCmp(at(row, col), 0);
    m_data->set(row, col, val);
    balance();
}

/**
 * @brief Balance data in order to achieve effective memory management.
 * When the matrix is sparse, save the data as map, otherwise save it as a two dimensional array.
 *
 */
void Matrix::balance()
{
    double threshold = SPARSE_THRESHOLD * m_shape_x * m_shape_y;
    bool wasBelowThreshold = lastZeroCount < threshold;
    bool isBelowThreshold = zeroCount < threshold;
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
