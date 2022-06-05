#include "Calculator.h"
#include "settings.h"

void Calculator::print(std::ostream &out)
{
    for (auto [name, matrix] : m_variables)
    {
        out << name << ":" << std::endl;
        matrix->print(out);
        out << std::endl;
    }
}

std::unique_ptr<Matrix> Calculator::execute(std::istream &in, std::ostream &out)
{

    std::string name = parser.parseIdentifier(in);
    if (name == PRINT_VARIABLES)
    {
        print(out);
    }
    else if (name == HELP)
    {
        out << HELP_MESSAGE;
    }
    else
    {
        parser.putback(in, name);
        return parser.parseInput(in);
    }
    parser.matchLeft(in, END_COMMAND);
    return nullptr;
}
