#pragma once
#include <iostream>
#include <string>
#include <set>
#include <list>
#include <memory>
#include <unordered_map>
#include <vector>
#include "../Operations/Operation.h"
#include "../Matrix/Matrix.h"

class Parser
{
private:
    // std::istream &in = std::cin;
    std::unordered_map<std::string, std::unique_ptr<Operation>> operations;

    void consumeWhite(std::istream &in) const;
    void putback(std::istream &is, const std::string &str) const;
    void match(std::istream &in, char c) const;

    std::string parseIdentifier(std::istream &in) const;
    std::string parseOperator(std::istream &in) const;

    double parseValue(std::istream &in) const;
    std::unique_ptr<Matrix> parseMatrix(std::istream &in) const;
    std::unique_ptr<Matrix> parseFactor(std::istream &in) const;

    std::unique_ptr<Matrix> parseExprRec(std::istream &in, std::unique_ptr<Matrix> lhs, int prio) const;
    std::unique_ptr<Matrix> parseExpression(std::istream &in, int prio) const;

public:
    // Parser(std::unordered_map<std::string, std::unique_ptr<Operation>> op) : operations(op){}
    std::unique_ptr<Matrix> parseInput(std::istream &in) const;
    std::unique_ptr<Matrix> parseInput(std::string &input) const;
};