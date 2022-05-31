#include "Index.h"
#include <vector>
#include "Matrix/Matrix.h"
#include "Operations/Operation.h"
#include "Parser/Parser.h"

int main()
{
    Parser p;
    std::vector<std::vector<double>> v;
    auto m = p.parseInput(std::cin)->evaluate();
    m->print(std::cout);
    std::cout << std::endl;
    std::cout << "Member at position [0, 0]: " << m->at(0, 0) << std::endl;
}