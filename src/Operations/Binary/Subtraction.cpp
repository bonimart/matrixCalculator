#include "Subtraction.h"
#include "Addition.h"

std::unique_ptr<Matrix> Subtraction::evaluate(Parameters p) const
{
    Addition a;
    *p.param2 = *p.param2 * -1;
    return a.evaluate(std::move(p));
}