#include "Selection.h"
#include "../../utils.h"
#include <iostream>

std::unique_ptr<Matrix> Selection::evaluate(Parameters p) const
{
    //! zvaliduj jestli jsou celociselny a je spravny rozmer

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