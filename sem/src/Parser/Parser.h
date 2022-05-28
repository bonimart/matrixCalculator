#pragma once
#include <iostream>
#include <string>
#include <set>
#include <list>
#include <memory>
#include <unordered_map>
#include "Operations/Operation.h"
#include "Matrix/Matrix.h"

class Parser
{
private:
    std::unordered_map<std::string, std::unique_ptr<Operation>> operations;

    void consumeWhite(std::istream &in) const;
    void putback(std::istream &is, const std::string &str) const;
    void match(std::istream &in, char c) const;

    std::string parseIdentifier(std::istream &in) const;
    std::string parseOperator(std::istream &in) const;

    double parseValue(std::istream &in) const;
    std::shared_ptr<Matrix> parseMatrix(std::istream &in) const;

public:
    Parser(std::unordered_map<std::string, std::unique_ptr<Operation>> op) : operations(op)
    {
    }
    std::unique_ptr<Operation> parse(std::istream &in) const;
    std::unique_ptr<Operation> parse(std::string &in) const;
};