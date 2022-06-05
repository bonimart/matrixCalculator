#include "Inversion.h"
#include "GEM.h"
#include "Rank.h"
#include "../Binary/Union.h"
#include "../Binary/Selection.h"

/**
 * @brief validate if input is regular matrix
 *
 * @param p
 */
void Inversion::validate(const Parameters &p) const
{
    if (p.paramCount != numOfOperands())
    {
        throw std::runtime_error("Matrix inversion takes only one argument");
    }
    Rank r;
    if ((p.param1->m_shape_x != p.param1->m_shape_y) ||
        (r.evaluate(std::make_unique<Matrix>(*p.param1))->at(0, 0) != p.param1->m_shape_x))
    {
        throw std::runtime_error("Inversion is undefined for singular matrices");
    }
}

/**
 * @brief calculate inverse matrix using gaussian elimination on given matrix joined with identity matrix
 *
 * @param p
 * @return std::unique_ptr<Matrix>
 */
std::unique_ptr<Matrix> Inversion::evaluate(Parameters p) const
{
    validate(p);

    GEM g;
    Union u;
    Selection s;
    std::size_t size = p.param1->m_shape_x;
    // identity matrix
    std::unique_ptr<Matrix> identityMat = std::make_unique<Matrix>(p.param1->m_shape_y,
                                                                   p.param1->m_shape_x);
    //? resime soustavu (A|E) - E - jednotkova matice
    std::unique_ptr<Matrix> unitedMatrices = u.evaluate({std::move(p.param1),
                                                         std::move(identityMat)});
    //? GEM prevede A na E, coz je pro regularni matice ekvivalentni nasobeni inverzi k A
    unitedMatrices = g.evaluate(std::move(unitedMatrices));
    //? soustava je nyni ve stavu (E|A^-1), nyni je treba vybrat jen A^-1
    Matrix selectionMat(std::vector<std::vector<double>>({{0, (double)size},
                                                          {(double)unitedMatrices->m_shape_y - 1, (double)unitedMatrices->m_shape_x - 1}}));
    std::unique_ptr<Matrix> m = std::make_unique<Matrix>(selectionMat);
    std::unique_ptr<Matrix> inv = s.evaluate({std::move(unitedMatrices),
                                              std::move(m)});
    return inv;
}