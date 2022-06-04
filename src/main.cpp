#include "Index.h"
#include <vector>
#include "settings.h"
#include "Matrix/Matrix.h"
#include "Operations/Operation.h"
#include "Parser/Parser.h"
#include "Calculator.h"
#include "Operations/Binary/Multiplication.h"
#include "Operations/Binary/Addition.h"
#include "Operations/Unary/Transposition.h"

int main()
{
    Calculator c;

    do
    {
        try
        {
            c.execute(std::cin).eof();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            while (std::cin.get() != END_COMMAND)
                ;
        }
    } while (!(std::cin.eof()));

    std::cout
        << std::endl;
    c.print(std::cout);
}