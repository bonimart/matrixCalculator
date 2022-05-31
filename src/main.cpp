#include "Index.h"
#include <vector>
#include "Matrix/Matrix.h"
#include "Operations/Operation.h"
#include "Parser/Parser.h"

int main()
{
    Parser p;
    std::vector<std::vector<double>> v;
    auto m = p.parseInput(std::cin);
    m->print(std::cout);
    std::cout << std::endl;
}