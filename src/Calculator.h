#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <map>
#include <memory>
#include <list>
#include <set>
#include "Matrix/Matrix.h"
#include "Parser/Parser.h"

class Calculator
{
private:
    std::map<std::string, std::shared_ptr<Matrix>, std::less<std::string>> m_variables = {};
    const std::unordered_map<std::string, std::shared_ptr<Operation>> m_operations;
    const std::unordered_map<std::string, int> m_operators;
    Parser parser;

public:
    Calculator(const std::unordered_map<std::string, std::shared_ptr<Operation>> &operations,
               const std::unordered_map<std::string, int> &operators)
        : m_operations(operations),
          m_operators(operators),
          parser(m_operations, m_operators, m_variables)
    {
    }
    void addMatrix(const std::shared_ptr<Matrix> m, const std::string &name);
    void addMatrix(std::ifstream &ifs);
    void exportMatrix(std::ofstream &ofs) const;

    std::unique_ptr<Matrix> execute(std::istream &in)
    {
        auto result = parser.parseInput(in);
        return result;
    }
    void print(std::ostream &out);
};