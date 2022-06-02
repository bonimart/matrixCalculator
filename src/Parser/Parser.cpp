//#pragma once
#include <vector>
#include <sstream>
#include "../Parser/Parser.h"
#include "../Matrix/Matrix.h"
#include "../settings.h"
#include "../Operations/Unary/Identity.h"
#include "../Operations/Binary/Addition.h"
#include "../Operations/Unary/Transposition.h"
#include "../Operations/Binary/Multiplication.h"

void Parser::consumeWhite(std::istream &in) const
{
    while (std::isspace(in.peek()) && in.peek() != END_COMMAND)
        in.get();
}

void Parser::putback(std::istream &in, const std::string &str) const
{
    for (auto it = str.rbegin(); it != str.rend(); it++)
        in.putback(*it);
}

void Parser::match(std::istream &in, char c) const
{
    if (in.peek() == EOF)
        return;
    consumeWhite(in);
    if (in.peek() != c)
    {
        throw std::runtime_error("Unexpected token '" +
                                 std::string(1, in.peek()) +
                                 "', expected " + std::string(1, c) + "'");
    }
    in.get();
    consumeWhite(in);
}

/**
 * @brief parses floating point number from given stream
 *
 * @param in stream to parse from
 * @return double parsed value
 */
double Parser::parseValue(std::istream &in) const
{
    /*
        double val = 0;
        while (std::isdigit(in.peek()))
        {
            val = val * 10 + (in.get() - '0');
        }
        // input value doesn't have decimal part
        if (in.peek() != DEC_POINT)
        {
            consumeWhite(in);
            return val;
        }
        // input value has decimal part
        in.get();
        double dec = 0;
        double exp = 1;
        while (std::isdigit(in.peek()))
        {
            dec = dec * 10 + (in.get() - '0');
            exp /= 10;
        }
        dec *= exp;
        val += dec;
        consumeWhite(in);
        return val;
    */
    double val;
    in >> val;
    consumeWhite(in);
    return val;
}

std::unique_ptr<Matrix> Parser::parseMatrix(std::istream &in) const
{
    match(in, L_MAT_PAR);
    std::vector<std::vector<double>> matrix;

    for (int shape_x = 0; in.peek() != R_MAT_PAR;)
    {
        std::vector<double> row;
        match(in, L_MAT_PAR);
        if (shape_x == 0)
        {
            for (; std::isdigit(in.peek()) || in.peek() == '-'; shape_x++)
            {
                row.push_back(parseValue(in));
                if (in.peek() != R_MAT_PAR)
                    match(in, DELIM);
            }
        }
        else
        {
            for (int i = 0; i < shape_x; i++)
            {
                row.push_back(parseValue(in));
                if (in.peek() == DELIM)
                    match(in, DELIM);
            }
        }
        match(in, R_MAT_PAR);
        matrix.push_back(row);
        if (in.peek() != R_MAT_PAR)
            match(in, DELIM);
    }
    match(in, R_MAT_PAR);

    return std::make_unique<Matrix>(matrix);
}

/**
 * @brief Parse identifier from input, only [a-zA-Z] allowed
 *
 * @param in input stream
 * @return std::string parsed identifier
 */
std::string Parser::parseIdentifier(std::istream &in) const
{
    std::string res;

    while (std::isalpha(in.peek()))
    {
        res += in.get();
    }

    consumeWhite(in);
    return res;
}

std::string Parser::parseOperator(std::istream &in) const
{
    std::string res;

    char c = in.peek();
    if (operators.count(std::string(1, c)) == 0)
    {
        throw std::runtime_error("Unexpected token '" + std::string(1, c) + "'");
    }
    in.get();
    res = std::string(1, c);
    consumeWhite(in);
    return res;
}

std::unique_ptr<Matrix> Parser::parseExpression(std::istream &in, int prio) const
{

    auto lhs = parseFactor(in);
    return parseExprRec(in, std::move(lhs), prio);
}

std::unique_ptr<Matrix> Parser::parseFactor(std::istream &in) const
{
    consumeWhite(in);
    if (in.peek() == EOF)
        return nullptr;
    // Matrix on input
    else if (in.peek() == L_MAT_PAR)
    {
        return parseMatrix(in);
    }
    // function argument on input
    else if (in.peek() == L_FUNC_PAR)
    {
        match(in, L_FUNC_PAR);
        auto res = parseExpression(in, 0);
        match(in, R_FUNC_PAR);
        return res;
    }
    // function or variable on input
    else if (std::isalpha(in.peek()))
    {
        // check for variable first - function has parantheses
        const std::string name = parseIdentifier(in);
        consumeWhite(in);
        // check for functions that are called as '{string} {operator} {matrix}' (=, <)
        if (operators.count(std::string(1, in.peek())) != 0 &&
            operators.at(std::string(1, in.peek())) == 0)
        {
            const std::string op = parseOperator(in);
            auto param = parseExpression(in, 0);
            return operations.at(op)->evaluate({std::move(param), name});
        }
        // variable is defined, return its value
        if (variables.count(name) != 0)
        {
            return std::make_unique<Matrix>(*(variables.at(name)));
        }
        // check for 'normal' functions {func}(*args)
        match(in, L_FUNC_PAR);
        if (operations.count(name) == 0)
        {
            throw std::runtime_error("parseFactor: Invalid function or variable name '" + name + "'");
        }
        // unary functions
        if (operations.at(name)->numOfOperands() == 1)
        {
            auto param = parseExpression(in, 0);
            auto res = operations.at(name)->evaluate(std::move(param));
            match(in, R_FUNC_PAR);
            return res;
        }
        // binary functions
        else if (operations.at(name)->numOfOperands() == 2)
        {
            auto param1 = parseExpression(in, 0);
            match(in, DELIM);
            auto param2 = parseExpression(in, 0);
            auto res = operations.at(name)->evaluate({std::move(param1), std::move(param2)});
            match(in, R_FUNC_PAR);
            return res;
        }
    }
    else
    {
        throw std::runtime_error("parseFactor: Expected '" +
                                 std::string(1, L_MAT_PAR) + "', '" +
                                 std::string(1, L_FUNC_PAR) +
                                 "' or valid identifier, got '" +
                                 std::string(1, in.peek()) + "'");
    }
    return nullptr;
}

std::unique_ptr<Matrix> Parser::parseExprRec(std::istream &in, std::unique_ptr<Matrix> lhs, int prio) const
{
    while (1)
    {
        if (in.peek() == R_FUNC_PAR || in.peek() == EOF || in.peek() == DELIM || in.peek() == END_COMMAND)
        {
            return lhs;
        }

        auto op = parseOperator(in);
        if (operators.at(op) < prio)
        {
            putback(in, op);
            return lhs;
        }

        auto rhs = parseFactor(in);
        if (in.peek() == R_FUNC_PAR || in.peek() == EOF || in.peek() == DELIM || in.peek() == END_COMMAND)
        {
            return operations.at(op)->evaluate({std::move(lhs), std::move(rhs)});
        }

        auto op2 = parseOperator(in);
        putback(in, op2);
        if (operators.at(op) < operators.at(op2))
        {
            rhs = parseExprRec(in, std::move(rhs), operators.at(op) + 1);
        }

        lhs = operations.at(op)->evaluate({std::move(lhs), std::move(rhs)});
    }
    return lhs;
}

std::unique_ptr<Matrix> Parser::parseInput(std::istream &in) const
{
    auto res = parseExpression(in, 0);
    match(in, END_COMMAND);
    return res;
}

std::unique_ptr<Matrix> Parser::parseInput(std::string &input) const
{
    std::stringstream ss(input);
    return parseExpression(ss, 0);
}