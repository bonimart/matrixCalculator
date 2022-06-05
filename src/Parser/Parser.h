#pragma once

#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <map>
#include <unordered_map>
#include "../Operations/Operation.h"
#include "../Matrix/Matrix.h"

class Parser
{
private:
    const std::unordered_map<std::string, std::shared_ptr<Operation>> operations;
    const std::unordered_map<std::string, int> operators;
    std::map<std::string, std::shared_ptr<Matrix>> &variables;
    // utilities
    void consumeWhite(std::istream &in) const;
    void match(std::istream &in, char c) const;
    //
    std::string parseFileName(std::istream &in) const;
    std::string parseOperator(std::istream &in) const;

    double parseValue(std::istream &in) const;
    std::unique_ptr<Matrix> parseMatrix(std::istream &in) const;
    std::unique_ptr<Matrix> parseDense(std::istream &in) const;
    std::unique_ptr<Matrix> parseSparse(std::istream &in) const;
    std::unique_ptr<Matrix> parseFactor(std::istream &in) const;

    std::unique_ptr<Matrix> parseExprRec(std::istream &in,
                                         std::unique_ptr<Matrix> lhs,
                                         int prio) const;
    std::unique_ptr<Matrix> parseExpression(std::istream &in, int prio) const;

public:
    Parser(const std::unordered_map<std::string, std::shared_ptr<Operation>> &op,
           const std::unordered_map<std::string, int> &opPrio,
           std::map<std::string, std::shared_ptr<Matrix>> &var)
        : operations(op), operators(opPrio), variables(var) {}

    void matchLeft(std::istream &in, char c) const;
    void putback(std::istream &is, const std::string &str) const;

    std::string parseIdentifier(std::istream &in) const;

    std::unique_ptr<Matrix> parseInput(std::istream &in) const;
    std::unique_ptr<Matrix> parseInput(std::string &input) const;
};
