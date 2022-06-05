#pragma once

#include <iostream>
#include "Unary.h"
#include "Identity.h"

/**
 * @brief class for printing matrices into output stream
 *
 */
class Print : public Unary
{
protected:
    std::ostream &out;

public:
    Print(std::ostream &os)
        : out(os) {}

    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override
    {
        Identity i;
        p.param1->print(out);
        out << std::endl;
        return i.evaluate(std::move(p.param1));
    }
};