#include <vector>
#include <sstream>
#include <fstream>
#include "../Parser/Parser.h"
#include "../settings.h"

/**
 * @brief consume whitespaces (doesn't skip END_COMMAND) from input stream
 *
 * @param in input stream
 */
void Parser::consumeWhite(std::istream &in) const
{
    while (std::isspace(in.peek()) && in.peek() != END_COMMAND)
        in.get();
}

/**
 * @brief puts a string back into input stream
 *
 * @param in input stream
 * @param str string to putback
 */
void Parser::putback(std::istream &in, const std::string &str) const
{
    for (auto it = str.rbegin(); it != str.rend(); it++)
        in.putback(*it);
}

void Parser::matchLeft(std::istream &in, char c) const
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
}

/**
 * @brief gets a char from input stream, compares to given value, throws exception if values don't match
 *
 * @param in input stream
 * @param c expected value
 */
void Parser::match(std::istream &in, char c) const
{
    matchLeft(in, c);
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
            if (shape_x == 0)
                throw std::runtime_error("Matrix cannot have empty rows");
        }
        else
        {
            for (int i = 0; i < shape_x; i++)
            {
                auto val = parseValue(in);
                if (!in)
                {
                    in.clear();
                    throw std::runtime_error("Invalid row");
                }
                row.push_back(val);
                if (i < shape_x - 1)
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

/**
 * @brief parses file name from given stream, file name satisfies {alpha}(.{alpha})
 *
 * @param in input stream
 * @return std::string
 */
std::string Parser::parseFileName(std::istream &in) const
{

    std::string res;

    while (std::isalpha(in.peek()) || in.peek() == '.' || in.peek() == '/')
    {
        res += in.get();
    }
    consumeWhite(in);
    return res;
}

/**
 * @brief gets operator, throws exception if operator is not defined
 *
 * @param in input stream
 * @return std::string
 */
std::string Parser::parseOperator(std::istream &in) const
{
    std::string res;

    char c = in.peek();
    if (operators.count(std::string(1, c)) == 0)
    {
        throw std::runtime_error("Unexpected token '" +
                                 std::string(1, c) + "'");
    }
    in.get();
    res = std::string(1, c);
    consumeWhite(in);
    return res;
}

/**
 * @brief recursively parses expression from input stream
 *
 * @param in input stream
 * @param prio priority of parsing
 * @return std::unique_ptr<Matrix> evaluated expression
 */
std::unique_ptr<Matrix> Parser::parseExpression(std::istream &in, int prio) const
{

    auto lhs = parseFactor(in);
    return parseExprRec(in, std::move(lhs), prio);
}

/**
 * @brief determines how to parse data given from input stream (except for binary matrix operators), then returns evaluated data
 *
 * @param in input stream
 * @return std::unique_ptr<Matrix> evaluated expression
 */
std::unique_ptr<Matrix> Parser::parseFactor(std::istream &in) const
{
    consumeWhite(in);
    if (in.peek() == EOF)
        return nullptr;
    // matrix on input
    else if (in.peek() == L_MAT_PAR)
    {
        return parseMatrix(in);
    }
    // existing variable on input
    else if (in.peek() == VAR_IDENTIFIER)
    {
        match(in, VAR_IDENTIFIER);
        const std::string name = parseIdentifier(in);
        if (variables.count(name) == 0)
        {
            throw std::runtime_error("Invalid variable name '" + name + "'");
        }
        return std::make_unique<Matrix>(*variables.at(name));
    }
    // name of input file on input
    else if (in.peek() == FILE_IDENTIFIER)
    {
        match(in, FILE_IDENTIFIER);
        const std::string fname = parseFileName(in);
        std::ifstream ifs(fname);
        if (!ifs.is_open())
        {
            throw std::runtime_error("Invalid file name '" + fname + "'");
        }
        auto res = parseExpression(ifs, 0);
        ifs.close();
        return res;
    }
    // functions, setting variables, writing into files
    else if (std::isalpha(in.peek()))
    {
        //? parseFileName because identifiers are always valid filenames, but filenames can be invalid identifiers
        std::string name = parseFileName(in);
        consumeWhite(in);
        // setting variables/writing into files is done with operators ('=', '<')
        if (operators.count(std::string(1, in.peek())) != 0 &&
            operators.at(std::string(1, in.peek())) == WRITE_PRIORITY)
        {
            const std::string op = parseOperator(in);
            auto res = parseExpression(in, 0);
            if (op == WRITE_TO_FILE)
            {
                std::ofstream ofs(name);
                if (!ofs.is_open())
                {
                    throw std::runtime_error("Could not open output file '" +
                                             name + "'");
                }
                res->print(ofs);
                ofs.close();
            }
            else if (op == SET_VAR)
            {
                putback(in, name);
                name = parseIdentifier(in);
                variables[name] = std::make_shared<Matrix>(*res);
            }
            else
            {
                throw std::runtime_error("Invalid operator '" + op + "'");
            }
            return res;
        }
        //?putback in case name had a '.' (identifiers do not allow '.')
        putback(in, name);
        name = parseIdentifier(in);
        // function is on input
        if (operations.count(name) == 0)
        {
            throw std::runtime_error("Invalid function name '" + name + "'");
        }
        match(in, L_FUNC_PAR);
        // unary functions
        if (operations.at(name)->numOfOperands() == 1)
        {
            auto param1 = parseExpression(in, 0);
            auto res = operations.at(name)->evaluate(std::move(param1));
            match(in, R_FUNC_PAR);
            return res;
        }
        // binary functions
        else if (operations.at(name)->numOfOperands() == 2)
        {
            // first parameter
            auto param1 = parseExpression(in, 0);
            match(in, DELIM);
            // second parameter
            auto param2 = parseExpression(in, 0);
            auto res = operations.at(name)->evaluate({std::move(param1),
                                                      std::move(param2)});
            match(in, R_FUNC_PAR);
            return res;
        }
    }
    else
    {
        throw std::runtime_error("Invalid identifier '" +
                                 std::string(1, in.peek()) + "'");
    }
    return nullptr;
}

std::unique_ptr<Matrix> Parser::parseExprRec(std::istream &in,
                                             std::unique_ptr<Matrix> lhs,
                                             int prio) const
{
    while (1)
    {
        if (in.peek() == R_FUNC_PAR ||
            in.peek() == EOF ||
            in.peek() == DELIM ||
            in.peek() == END_COMMAND)
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
        if (in.peek() == R_FUNC_PAR ||
            in.peek() == EOF ||
            in.peek() == DELIM ||
            in.peek() == END_COMMAND)
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
    matchLeft(in, END_COMMAND);
    return res;
}

std::unique_ptr<Matrix> Parser::parseInput(std::string &input) const
{
    std::stringstream ss(input);
    return parseExpression(ss, 0);
}