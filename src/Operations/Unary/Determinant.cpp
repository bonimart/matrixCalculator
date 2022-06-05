#include "Determinant.h"

/**
 * @brief checks if argument is a square matrix
 *
 * @param p
 */
void Determinant::validate(const Parameters &p) const
{
    if (p.paramCount != numOfOperands())
    {
        throw std::runtime_error("Determinant takes one matrix");
    }
    if (p.param1->m_shape_x != p.param1->m_shape_y)
    {
        throw std::runtime_error("Determinant is defined for square matrices only");
    }
}

/**
 * @brief evaluates determinant from definition with recursion
 *
 * @param p
 * @return std::unique_ptr<Matrix>
 */
std::unique_ptr<Matrix> Determinant::evaluate(Parameters p) const
{
    validate(p);
    std::shared_ptr<Matrix> m = std::move(p.param1);
    std::vector<int> perm;
    for (std::size_t i = 0; i < m->m_shape_x; ++i)
    {
        perm.push_back(i);
    }
    double det = detRec(m, 1, perm);
    return std::make_unique<Matrix>(1, 1, det);
}

/**
 * @brief recursive part of determinant calculation, determinant is a sum of products through all possible permutations.
 *
 * @param m matrix, its determinant is to be calculated
 * @param prod product of elements of unique permutation that accumulates with the process of recursion
 * @param permutation indeces chosen so far
 * @return double
 */
double Determinant::detRec(const std::shared_ptr<Matrix> m,
                           double prod,
                           const std::vector<int> &permutation) const
{
    double sum = 0;
    if (permutation.empty())
    {
        return prod;
    }
    else
    {
        for (auto it = permutation.begin(); it != permutation.end(); ++it)
        {
            // only if the first element is chosen, the sign of determinant doesn't change
            int sgn = -1;
            if (it == permutation.begin())
            {
                sgn = 1;
            }
            // pass permutations without chosen index to next step in recursion
            std::vector<int> perm;
            for (auto jt = permutation.begin(); jt != permutation.end(); ++jt)
            {
                if (jt != it)
                    perm.push_back(*jt);
            }
            sum += detRec(m,
                          sgn * prod * m->at(*it, m->m_shape_x - permutation.size()),
                          perm);
        }
    }
    return sum;
}