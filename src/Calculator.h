#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <map>
#include <memory>
#include <list>
#include <set>
#include "Matrix/Matrix.h"
#include "Operations/Operation.h"
#include "Operations/Unary/Print.h"
#include "Operations/Unary/GEM.h"
#include "Operations/Unary/Rank.h"
#include "Operations/Unary/Inversion.h"
#include "Operations/Unary/AddVar.h"
#include "Operations/Unary/Import.h"
#include "Operations/Unary/Determinant.h"
#include "Operations/Binary/Addition.h"
#include "Operations/Binary/Export.h"
#include "Operations/Binary/Subtraction.h"
#include "Operations/Binary/Multiplication.h"
#include "Operations/Binary/Selection.h"
#include "Operations/Binary/Union.h"
#include "Parser/Parser.h"

class Calculator
{
private:
    std::map<std::string, std::shared_ptr<Matrix>, std::less<std::string>> variables = {};
    const std::unordered_map<std::string, std::shared_ptr<Operation>> operations = {
        std::make_pair<std::string, std::shared_ptr<Operation>>("tr", std::make_shared<Transposition>()),
        std::make_pair<std::string, std::shared_ptr<Operation>>("det", std::make_shared<Determinant>()),
        std::make_pair<std::string, std::shared_ptr<Operation>>("print", std::make_shared<Print>(std::cout)),
        std::make_pair<std::string, std::shared_ptr<Operation>>("import", std::make_shared<Import>()),
        std::make_pair<std::string, std::shared_ptr<Operation>>("gem", std::make_shared<GEM>()),
        std::make_pair<std::string, std::shared_ptr<Operation>>("rank", std::make_shared<Rank>()),
        std::make_pair<std::string, std::shared_ptr<Operation>>("inv", std::make_shared<Inversion>()),
        std::make_pair<std::string, std::shared_ptr<Operation>>("sel", std::make_shared<Selection>()),
        std::make_pair<std::string, std::shared_ptr<Operation>>("join", std::make_shared<Union>()),
        std::make_pair<std::string, std::shared_ptr<Operation>>("+", std::make_shared<Addition>()),
        std::make_pair<std::string, std::shared_ptr<Operation>>("-", std::make_shared<Subtraction>()),
        std::make_pair<std::string, std::shared_ptr<Operation>>("*", std::make_shared<Multiplication>()),
        std::make_pair<std::string, std::shared_ptr<Operation>>("=", std::make_shared<AddVar>(variables)),
        std::make_pair<std::string, std::shared_ptr<Operation>>("<", std::make_shared<Export>())};
    const std::unordered_map<std::string, int> operators = {
        {"=", 0},
        {"<", 0},
        {"+", 1},
        {"-", 1},
        {"*", 2}};
    Parser parser;

    class ExportVariable;

public:
    Calculator()
        : parser(operations, operators, variables)
    {
    }
    void addMatrix(const std::shared_ptr<Matrix> m, const std::string &name);
    void addMatrix(std::ifstream &ifs);
    void exportMatrix(std::ofstream &ofs) const;

    // zparsuje jeden radek
    std::istream &execute(std::istream &in)
    {
        parser.parseInput(in);
        return in;
    }
    void print(std::ostream &out);
};