#include "Inversion.h"
#include "GEM.h"
#include "Rank.h"
#include "../Binary/Union.h"
#include "../Binary/Selection.h"
#include "../../utils.h"
#include <iostream>

std::unique_ptr<Matrix> Inversion::evaluate(Parameters p) const
{
    //! musi byt ctvercova a rank n
    GEM g;
    Union u;
    Selection s;
    std::size_t size = p.param1->m_shape_x;
    // TODO lepsi vypocet identity matrix
    std::unique_ptr<Matrix> identityMat = g.evaluate(std::make_unique<Matrix>(*p.param1));
    //? resime soustavu (A|E) - E - jednotkova matice
    std::unique_ptr<Matrix> unitedMatrices = u.evaluate({std::move(p.param1), std::move(identityMat)});
    //? GEM prevede A na E, coz je pro regularni matice ekvivalentni nasobeni inverzi k A
    unitedMatrices = g.evaluate(std::move(unitedMatrices));
    //? soustava je nyni ve stavu (E|A^-1), nyni je treba vybrat jen A^-1
    Matrix selectionMat({{0, (double)size},
                         {(double)unitedMatrices->m_shape_y - 1, (double)unitedMatrices->m_shape_x - 1}});
    std::unique_ptr<Matrix> m = std::make_unique<Matrix>(selectionMat);
    std::unique_ptr<Matrix> inv = s.evaluate({std::move(unitedMatrices), std::move(m)});
    return inv;
}