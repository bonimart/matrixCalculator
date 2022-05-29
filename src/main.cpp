#include "Index.h"
#include "Matrix/Matrix.h"
#include "Operations/Operation.h"
#include "Parser/Parser.h"

int main()
{

    Parser p;
    std::vector<std::vector<double>> m = p.parseMatrix(std::cin);
    std::cout << m[0][0] << std::endl;
}