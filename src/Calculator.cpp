#include "Calculator.h"

void Calculator::print(std::ostream &out)
{
    for (auto [name, matrix] : m_variables)
    {
        out << name << ":" << std::endl;
        matrix->print(out);
        out << std::endl;
    }
}
