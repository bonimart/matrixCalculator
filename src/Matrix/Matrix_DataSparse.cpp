#include "Matrix.h"
#include "../utils.h"

Matrix::DataSparse::DataSparse(const std::shared_ptr<Data> data, std::size_t shape_y, std::size_t shape_x)
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

double Matrix::DataSparse::at(std::size_t i, std::size_t j) const
{
    if (m_data.count(i) == 0 || m_data.at(i).count(j) == 0)
        return 0;
    return m_data.at(i).at(j);
}

void Matrix::DataSparse::set(std::size_t i, std::size_t j, double val)
{
    if (doubleCmp(val, 0))
    {
        if (m_data.count(i) != 0 && m_data.at(i).count(j) != 0)
        {
            m_data.at(i).erase(j);
            if (m_data.at(i).empty())
                m_data.erase(i);
        }
        return;
    }
    m_data[i][j] = val;
}

void Matrix::DataSparse::print(std::ostream &out, std::size_t shape_y, std::size_t shape_x) const
{
    out << L_MAT_PAR;
    bool firstPrint = true;
    for (std::size_t i = 0; i < shape_y; ++i)
    {
        if (m_data.count(i) == 0)
            continue;
        if (!firstPrint)
        {
            out << "," << std::endl
                << " ";
        }
        firstPrint = true;
        for (std::size_t j = 0; j < shape_x; ++j)
        {
            if (m_data.count(i) != 0 && m_data.at(i).count(j) != 0)
            {
                if (!firstPrint)
                    out << ", ";
                else
                    firstPrint = false;
                out << "{" << i << "," << j << "}:" << this->at(i, j);
            }
        }
    }
    out << R_MAT_PAR;
    /*
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
     */
}

std::shared_ptr<Matrix::Data> Matrix::DataSparse::clone() const
{
    return std::make_shared<Matrix::DataSparse>(m_data);
}