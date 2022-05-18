#include "Matrix/Matrix.h"

bool Matrix::validIndex(const Index &coords) const
{
    return coords.x >= 0 && coords.x < m_shape.x && coords.y >= 0 && coords.y < m_shape.y;
}