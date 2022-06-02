#include "Calculator.h"
#include "Operations/Identity.h"

void Calculator::print(std::ostream &out)
{
    for (auto [name, matrix] : variables)
    {
        out << name << ":" << std::endl;
        matrix->print(out);
        out << std::endl;
    }
}

void Calculator::addMatrix(const std::shared_ptr<Matrix> m, const std::string &name)
{
    variables[name] = m;
}