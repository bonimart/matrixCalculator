//#pragma once
#include <vector>
#include <sstream>
#include "../Parser/Parser.h"
#include "../Matrix/Matrix.h"
#include "../settings.h"

void Parser::consumeWhite(std::istream &in) const
{
    while (std::isspace(in.peek()))
        in.get();
}

void Parser::putback(std::istream &in, const std::string &str) const
{
    for (auto it = str.rbegin(); it != str.rend(); it++)
        in.putback(*it);
}

void Parser::match(std::istream &in, char c) const
{
    consumeWhite(in);
    if (in.peek() != c)
    {
        throw std::runtime_error("Unexpected token '" + std::string(1, in.peek()) + ", expected " + std::string(1, c));
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
    int exp = 1;
    while (std::isdigit(in.peek()))
    {
        dec = dec * 10 + (in.get() - '0');
        exp /= 10;
    }
    dec *= exp;
    val += dec;
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
            for (; std::isdigit(in.peek()); shape_x++)
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
    if (operations.count(std::string(1, c)) == 0)
    {
        throw std::runtime_error("Unexpected token " + std::string(1, c));
    }
    res = std::string(1, c);
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
    if (in.peek() == L_MAT_PAR)
    {
        return parseMatrix(in);
    }
    else
    {
        throw std::runtime_error("parseFactor: Expected '" + std::string(1, L_MAT_PAR) + "'");
    }
}

std::unique_ptr<Matrix> Parser::parseExprRec(std::istream &in, std::unique_ptr<Matrix> lhs, int prio) const
{
    return lhs;
}

std::unique_ptr<Matrix> Parser::parseInput(std::istream &in) const
{
    return parseExpression(in, 0);
}

std::unique_ptr<Matrix> Parser::parseInput(std::string &input) const
{
    std::stringstream ss(input);
    return parseExpression(ss, 0);
}