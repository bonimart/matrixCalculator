#include <cassert>
#include <sstream>
//#include "../src/Calculator.h"
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
    /*
    std::unique_ptr<Matrix> m4 = std::make_unique<Matrix>(Matrix({{1, 2, 3}}));
    assert(m4->m_shape_x == 3 && m4->m_shape_y == 1);
    Transposition tr;
    auto m5 = tr.evaluate({std::make_unique<Matrix>(m4)});
    assert(m5->m_shape_x == m4->m_shape_y && m5->m_shape_y == m4->m_shape_x);
    for (int i = 0; i < 3; ++i)
    {
        assert(m5->at(i, 0) == m4->at(0, i));
    }

    std::ostringstream ss1;
    std::ostringstream ss2;
    Print p(ss1);
    auto m6 = p.evaluate({std::move(m4)});
    m5->print(ss2);
    ss2 << std::endl;
    assert(ss1.str() == ss2.str());

    GEM g;
    g.evaluate(std::make_unique<Matrix>());
    */
    return EXIT_SUCCESS;
}