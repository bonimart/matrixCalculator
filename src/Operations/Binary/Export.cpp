#include "Export.h"
#include "../Unary/Print.h"
#include <fstream>

void Export::validate(const Parameters &p) const
{
    std::ofstream of(p.param_str);
    if (!of.is_open())
    {
        throw std::runtime_error("Cannot access export file");
    }
    of.close();
}

std::unique_ptr<Matrix> Export::evaluate(Parameters p) const
{
    //! validate urci, jestli existuje soubor
    std::ofstream out(p.param_str);
    Print pr(out);
    auto ret = pr.evaluate(std::move(p.param1));
    out.close();
    return ret;
}