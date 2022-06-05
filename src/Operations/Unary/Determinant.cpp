#include "Determinant.h"

void Determinant::validate(const Parameters &p) const
{
    if (p.param1->m_shape_x != p.param1->m_shape_y)
    {
        throw std::runtime_error("Determinant is defined for square matrices only");
    }
}

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
            int sgn = -1;
            if (it == permutation.begin())
            {
                sgn = 1;
            }
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