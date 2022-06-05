#include "Matrix.h"
#include "../utils.h"
#include "../settings.h"

/**
 * @brief Construct a new Matrix:: Data Sparse:: Data Sparse object
 *
 * @param data
 * @param shape_y
 * @param shape_x
 */
Matrix::DataSparse::DataSparse(const std::shared_ptr<Data> data,
                               std::size_t shape_y,
                               std::size_t shape_x)
{
    for (std::size_t i = 0; i < shape_y; ++i)
    {
        for (std::size_t j = 0; j < shape_x; ++j)
        {
            if (!doubleCmp(data->at(i, j), 0))
                m_data[i][j] = data->at(i, j);
        }
    }
}

/**
 * @brief Construct a new Matrix:: Data Sparse:: Data Sparse object
 *
 * @param data
 */
Matrix::DataSparse::DataSparse(const std::vector<std::vector<double>> &data)
{
    for (std::size_t i = 0; i < data.size(); ++i)
    {
        for (std::size_t j = 0; j < data.at(0).size(); ++j)
        {
            if (!doubleCmp(data.at(i).at(j), 0))
                m_data[i][j] = data.at(i).at(j);
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
double Matrix::DataSparse::at(std::size_t row, std::size_t col) const
{
    if (m_data.count(row) == 0 || m_data.at(row).count(col) == 0)
        return 0;
    return m_data.at(row).at(col);
}

/**
 * @brief set value at given position
 *
 * @param row
 * @param col
 * @param val
 */
void Matrix::DataSparse::set(std::size_t row, std::size_t col, double val)
{
    if (doubleCmp(val, 0))
    {
        if (m_data.count(row) != 0 && m_data.at(row).count(col) != 0)
        {
            m_data.at(row).erase(col);
            if (m_data.at(row).empty())
                m_data.erase(row);
        }
        return;
    }
    m_data[row][col] = val;
}

#ifdef DIFFERENT_SPARSE_PRINT
/**
 * @brief print sparse matrix to output stream
 *
 * @param out
 * @param shape_y
 * @param shape_x
 */
void Matrix::DataSparse::print(std::ostream &out,
                               std::size_t shape_y,
                               std::size_t shape_x) const
{
    out << L_MAT_PAR;
    bool firstPrint = true;
    for (std::size_t i = 0; i < shape_y; ++i)
    {
        if (m_data.count(i) == 0)
            continue;
        if (!firstPrint)
        {
            out << DELIM << std::endl
                << " ";
        }
        firstPrint = true;
        for (std::size_t j = 0; j < shape_x; ++j)
        {
            if (m_data.count(i) != 0 && m_data.at(i).count(j) != 0)
            {
                if (!firstPrint)
                    out << DELIM << " ";
                else
                    firstPrint = false;
                out << L_SPARSE_INDEX << i << DELIM << j << R_SPARSE_INDEX << INDEX_VAL_DELIM << this->at(i, j);
            }
        }
    }
    out << R_MAT_PAR;
}
#else
void Matrix::DataSparse::print(std::ostream &out,
                               std::size_t shape_y,
                               std::size_t shape_x) const
{
    out << L_MAT_PAR;
    for (std::size_t i = 0; i < shape_y; ++i)
    {
        if (i != 0)
            out << DELIM << std::endl
                << " ";
        out << L_MAT_PAR;
        for (std::size_t j = 0; j < shape_x; ++j)
        {
            if (j != 0)
                out << DELIM << " ";
            out << this->at(i, j);
        }
        out << R_MAT_PAR;
    }
    out << R_MAT_PAR;
}
#endif

/**
 * @brief make a copy of sparse matrix
 *
 * @return std::shared_ptr<Matrix::Data>
 */
std::shared_ptr<Matrix::Data> Matrix::DataSparse::clone() const
{
    return std::make_shared<Matrix::DataSparse>(m_data);
}
