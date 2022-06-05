#include "Multiplication.h"

/**
 * @brief check if number of columns of first matrix is the same as number of rows of the second
 *
 * @param p
 */
void Multiplication::validate(const Parameters &p) const
{
    if (p.paramCount != numOfOperands())
    {
        throw std::runtime_error("Matrix multiplication takes two arguments");
    }
    if (p.param1->m_shape_y != p.param2->m_shape_x)
    {
        throw std::runtime_error("Matrix multiplication is defined for matrices with following shapes [l,m]*[m,n]");
    }
}

/**
 * @brief matrix multiplication of two matrices
 *
 * @param p
 * @return std::unique_ptr<Matrix>
 */
std::unique_ptr<Matrix> Multiplication::evaluate(Parameters p) const
{
    validate(p);
    std::unique_ptr<Matrix> m1 = std::move(p.param1);
    std::unique_ptr<Matrix> m2 = std::move(p.param2);
    std::unique_ptr<Matrix> res = std::make_unique<Matrix>(m1->m_shape_y,
                                                           m2->m_shape_x,
                                                           0);
    for (std::size_t i = 0; i < m1->m_shape_y; ++i)
    {
        for (std::size_t j = 0; j < m2->m_shape_x; ++j)
        {
            double sum = 0;
            for (std::size_t k = 0; k < m1->m_shape_x; ++k)
            {
                sum += m1->at(i, k) * m2->at(k, j);
            }
            res->set(i, j, sum);
        }
    }
    return res;
}