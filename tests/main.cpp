#include <cassert>
#include <sstream>
#include <map>
#include "../src/Calculator.h"
#include "../src/Parser/Parser.h"
#include "../src/utils.h"
#include "../src/Operations/operations.h"
#include "../src/Matrix/Matrix.h"
#include "../src/Operations/Unary/Print.h"
#include "../src/Operations/Unary/GEM.h"
#include "../src/Operations/Unary/Rank.h"
#include "../src/Operations/Unary/Inversion.h"
#include "../src/Operations/Unary/Determinant.h"
#include "../src/Operations/Unary/Transposition.h"
#include "../src/Operations/Binary/Addition.h"
#include "../src/Operations/Binary/Subtraction.h"
#include "../src/Operations/Binary/Multiplication.h"
#include "../src/Operations/Binary/Selection.h"
#include "../src/Operations/Binary/Union.h"

int main()
{
    // utilities check
    double d1 = 1.5;
    double d2 = 1.50000000001;
    assert(!doubleCmp(d1, d2));
    assert(doubleCmp(d1, d1));

    // Basic matrix operations
    Matrix m({{1, 0}, {0, 1}});
    assert(m.at(0, 0) == 1);
    assert(m.at(1, 0) == 0);
    m.set(1, 0, 1);
    assert(m.at(1, 0) == 1);
    assert(m.m_shape_x == m.m_shape_y && m.m_shape_x == 2);
    // Matrix filled with given values
    Matrix m2(2, 2, 0);
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            assert(m2.at(i, j) == 0);
        }
    }
    // Identity matrix
    Matrix m3(2, 2);
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            assert(m3.at(i, j) == (i == j));
        }
    }

    Matrix m4({{1, 2, 3}, {4, 5, 6}});
    assert(m4.m_shape_x == 3 && m4.m_shape_y == 2);
    Transposition tr;
    auto m5 = tr.evaluate({std::make_unique<Matrix>(m4)});
    assert(m5->m_shape_x == m4.m_shape_y && m5->m_shape_y == m4.m_shape_x);
    for (int i = 0; i < 3; ++i)
    {
        assert(m5->at(i, 0) == m4.at(0, i));
    }

    std::ostringstream ss1;
    std::ostringstream ss2;
    Print p(ss1);
    auto m6 = p.evaluate(std::make_unique<Matrix>(*m5));
    m5->print(ss2);
    ss2 << std::endl;
    assert(ss1.str() == ss2.str());

    GEM g;
    auto m7 = g.evaluate(std::make_unique<Matrix>(m4));
    assert(m7->at(0, 0) == 1);
    assert(m7->at(0, 1) == 0);
    assert(m7->at(1, 0) == 0);
    assert(m7->at(1, 1) == 1);

    Rank r;
    assert(r.evaluate({std::move(m7)})->at(0, 0) == r.evaluate(std::make_unique<Matrix>(m4))->at(0, 0));

    Determinant d;
    try
    {
        d.evaluate(std::make_unique<Matrix>(m4));
        // exception should be thrown;
        assert(1 == 0);
    }
    catch (const std::runtime_error &e)
    {
        assert(e.what() == std::string("Determinant is defined for square matrices only"));
    }
    Matrix m9({{42, 0}, {0, 1}});
    assert(d.evaluate(std::make_unique<Matrix>(m9))->at(0, 0) == 42);

    Inversion inv;
    try
    {
        inv.evaluate(std::make_unique<Matrix>(m4));
        // exception should be thrown;
        assert(1 == 0);
    }
    catch (const std::runtime_error &e)
    {
        assert(e.what() == std::string("Inversion is undefined for singular matrices"));
    }
    assert(inv.evaluate(std::make_unique<Matrix>(m9))->at(0, 0) == 1 / 42.);

    Addition a;
    assert(a.evaluate({std::make_unique<Matrix>(m9), std::make_unique<Matrix>(m9)})->at(0, 0) == 84);

    Subtraction s;
    assert(s.evaluate({std::make_unique<Matrix>(m9), std::make_unique<Matrix>(m9)})->at(0, 0) == 0);

    Matrix m10(std::vector<std::vector<double>>({{1, 2, 3}}));
    Multiplication mult;
    assert(mult.evaluate({std::make_unique<Matrix>(m10), tr.evaluate(std::make_unique<Matrix>(m10))})->at(0, 0) == 14);

    Union u;
    auto m11 = u.evaluate({std::make_unique<Matrix>(m10), std::make_unique<Matrix>(m10)});
    assert(m11->m_shape_x == 6 && m11->m_shape_y == m10.m_shape_y);
    assert(m11->at(0, 0) == m11->at(0, 3));

    Selection sel;

    auto m12 = sel.evaluate({std::move(m11),
                             std::make_unique<Matrix>(std::vector<std::vector<double>>({{0, 0}, {0, 1}}))});
    assert(m12->m_shape_x == 2 && m12->m_shape_y == 1);

    std::map<std::string, std::shared_ptr<Matrix>> variables;
    Parser parser(operations, operators, variables);
    // Empty matrix
    try
    {
        parser.parseInput(std::string("[[]]"));
        // exception should be thrown;
        assert(1 == 0);
    }
    catch (const std::exception &e)
    {
        assert(e.what() == std::string("Matrix cannot have empty rows"));
    }
    // rows are not separated by ','
    try
    {

        parser.parseInput(std::string("[[1,2][1,2]]"));
        // exception should be thrown;
        assert(1 == 0);
    }
    catch (const std::exception &e)
    {
        assert(e.what() == std::string("Unexpected token '[', expected ','"));
    }
    // number of elements in next rows doesn't match number of elements in first row
    try
    {

        parser.parseInput(std::string("[[1,2],[1,]]"));
        // exception should be thrown;
        assert(1 == 0);
    }
    catch (const std::exception &e)
    {
        // std::cerr << e.what();
        assert(e.what() == std::string("Invalid row"));
    }

    parser.parseInput(std::string("a=[[1]];"));
    // variable is called without '$'
    try
    {

        parser.parseInput(std::string("a;"));
        // exception should be thrown;
        assert(1 == 0);
    }
    catch (const std::exception &e)
    {
        assert(e.what() == std::string("Invalid function name 'a'"));
    }
    auto m13 = parser.parseInput(std::string("$a;"));
    assert(m13->m_shape_x == m13->m_shape_y &&
           m13->m_shape_x == 1 &&
           m13->at(0, 0) == 1);

    auto m14 = parser.parseInput(std::string("singular=@../examples/ones.txt;"));
    // singularity check
    assert(m14->m_shape_x != m14->m_shape_y || m14->m_shape_x > r.evaluate(std::move(m14))->at(0, 0));

    auto m15 = parser.parseInput(std::string("regular=@../examples/regular.txt;"));
    // regularity check
    assert(m15->m_shape_x == m15->m_shape_y && m15->m_shape_x == r.evaluate(std::move(m15))->at(0, 0));

    return EXIT_SUCCESS;
}