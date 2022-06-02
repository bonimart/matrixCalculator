#include "Index.h"
#include <vector>
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

    while (!(c.execute(std::cin).eof()))
    {
    }

    std::cout << std::endl;
    c.print(std::cout);
}