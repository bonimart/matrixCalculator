#include "Calculator.h"
#include "settings.h"

/**
 * @brief print all variables of a calculator into given stream
 *
 * @param out
 */
void Calculator::print(std::ostream &out) const
{
    for (auto [name, matrix] : m_variables)
    {
        out << name << ":" << std::endl;
        matrix->print(out);
        out << std::endl;
    }
}

/**
 * @brief execute a command
 *
 * @param in input stream
 * @param out output stream
 * @return std::unique_ptr<Matrix>
 */
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
