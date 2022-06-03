#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <memory>
#include <list>
#include <set>
#include "Matrix/Matrix.h"
#include "Operations/Operation.h"
#include "Operations/Unary/Print.h"
#include "Operations/Binary/Addition.h"
#include "Operations/Binary/Multiplication.h"
#include "Operations/Unary/Determinant.h"
#include "Parser/Parser.h"

class AddVariable : public Operation
{
protected:
    std::unordered_map<std::string, std::shared_ptr<Matrix>> &variables;

public:
    AddVariable(std::unordered_map<std::string, std::shared_ptr<Matrix>> &var)
        : variables(var) {}
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override
    {
        variables[p.param_str] = std::make_shared<Matrix>(*(p.param1));
        return std::move(p.param1);
    }
    virtual int numOfOperands() const override
    {
        return 2;
    }
    virtual bool validate(const Parameters &p) const override { return true; }
};

class Calculator
{
private:
    std::unordered_map<std::string, std::shared_ptr<Matrix>> variables = {};
    const std::unordered_map<std::string, std::shared_ptr<Operation>> operations = {
        std::make_pair<std::string, std::shared_ptr<Operation>>("tr", std::make_shared<Transposition>()),
        std::make_pair<std::string, std::shared_ptr<Operation>>("det", std::make_shared<Determinant>()),
        std::make_pair<std::string, std::shared_ptr<Operation>>("print", std::make_shared<Print>(std::cout)),
        std::make_pair<std::string, std::shared_ptr<Operation>>("+", std::make_shared<Addition>()),
        std::make_pair<std::string, std::shared_ptr<Operation>>("*", std::make_shared<Multiplication>()),
        std::make_pair<std::string, std::shared_ptr<Operation>>("=", std::make_shared<AddVariable>(variables))};
    const std::unordered_map<std::string, int> operators = {
        {"=", 0},
        {"+", 1},
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