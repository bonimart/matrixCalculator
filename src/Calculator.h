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

/**
 * @brief calculator class that stores variables from computations
 *
 */
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

    void print(std::ostream &out);
    std::unique_ptr<Matrix> execute(std::istream &in, std::ostream &out);
};
