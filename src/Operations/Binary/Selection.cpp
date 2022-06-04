#include "Selection.h"
#include "../../utils.h"
#include <iostream>
#include <cmath>

void Selection::validate(const Parameters &p) const
{
    if (p.param2->m_shape_x != p.param2->m_shape_y || p.param2->m_shape_x != 2)
    {
        throw std::runtime_error("Selection takes matrix with 2 points: start-end");
    }
    for (std::size_t i = 0; i < p.param2->m_shape_y; ++i)
    {
        for (std::size_t j = 0; j < p.param2->m_shape_y; ++j)
        {
            double index = p.param2->at(i, j);
            if (!doubleCmp(index, std::round(index)))
            {
                throw std::runtime_error("Selection index is not an integer");
            }
            else if (j == 0 && (index < 0 || index > p.param1->m_shape_y))
            {
                throw std::runtime_error("Index is out of bounds");
            }
            else if (j == 1 && (index < 0 || index > p.param1->m_shape_x))
            {
                throw std::runtime_error("Index is out of bounds");
            }
        }
    }
}

std::unique_ptr<Matrix> Selection::evaluate(Parameters p) const
{
    //! zvaliduj jestli jsou celociselny a je spravny rozmer
    validate(p);

    std::size_t up = std::min(p.param2->at(0, 0), p.param2->at(1, 0));
    std::size_t bottom = std::max(p.param2->at(0, 0), p.param2->at(1, 0));
    std::size_t left = std::min(p.param2->at(0, 1), p.param2->at(1, 1));
    std::size_t right = std::max(p.param2->at(0, 1), p.param2->at(1, 1));

    std::unique_ptr<Matrix> m = std::make_unique<Matrix>(bottom - up + 1, right - left + 1);
    for (std::size_t i = up; i <= bottom; ++i)
    {
        for (std::size_t j = left; j <= right; ++j)
        {
            m->set(i - up, j - left, p.param1->at(i, j));
        }
    }
    return m;
}