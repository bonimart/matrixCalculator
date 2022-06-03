#include "Export.h"
#include "../Unary/Print.h"
#include <fstream>

std::unique_ptr<Matrix> Export::evaluate(Parameters p) const
{
    //! validate urci, jestli existuje soubor
    std::ofstream out(p.param_str);
    Print pr(out);
    auto ret = pr.evaluate(std::move(p.param1));
    out.close();
    return ret;
}