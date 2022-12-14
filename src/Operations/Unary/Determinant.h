#pragma once

#include "Unary.h"

/**
 * @brief Class for calculating determinant of a square matrix
 *
 */
class Determinant : public Unary
{
protected:
    double detRec(const std::shared_ptr<Matrix> m,
                  double det,
                  const std::vector<int> &permutation) const;

public:
    virtual void validate(const Parameters &p) const override;
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override;
};