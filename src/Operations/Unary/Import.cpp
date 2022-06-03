#include "Import.h"
#include <fstream>

void Import::consumeWhite(std::istream &in) const
{
    while (std::isspace(in.peek()))
        in.get();
}

void Import::match(std::istream &in, char c) const
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

std::unique_ptr<Matrix> Import::evaluate(Parameters p) const
{
    std::ifstream in(p.param_str);
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
                double d;
                in >> d;
                row.push_back(d);
                if (in.peek() != R_MAT_PAR)
                    match(in, DELIM);
            }
        }
        else
        {
            for (int i = 0; i < shape_x; i++)
            {
                double d;
                in >> d;
                row.push_back(d);
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
    in.close();
    return std::make_unique<Matrix>(matrix);
}