#pragma once
#include <iostream>
#include <string>
#include <set>
#include <list>
#include <memory>
#include <map>
#include "unordered_map"
#include <vector>
#include "../Operations/Operation.h"
#include "../Operations/Identity.h"
#include "../Operations/Unary/Transposition.h"
#include "../Matrix/Matrix.h"

class Parser
{
private:
    const std::unordered_map<std::string, std::shared_ptr<Operation>> operations;
    const std::unordered_map<std::string, int> operators;
    std::unordered_map<std::string, std::shared_ptr<Matrix>> &variables;

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
    Parser(const std::unordered_map<std::string, std::shared_ptr<Operation>> &op,
           const std::unordered_map<std::string, int> &opPrio,
           std::unordered_map<std::string, std::shared_ptr<Matrix>> &var)
        : operations(op), operators(opPrio), variables(var) {}
    std::unique_ptr<Matrix> parseInput(std::istream &in) const;
    std::unique_ptr<Matrix> parseInput(std::string &input) const;
};