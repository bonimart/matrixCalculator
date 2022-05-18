#include "Matrix/MatrixDense.h"

double MatrixDense::get(const Index &coords) const
{
    return m_data.at(coords.x).at(coords.y);
}
void MatrixDense::set(const Index &coords, const double &val)
{
    m_data.at(coords.x).at(coords.y) = val;
}