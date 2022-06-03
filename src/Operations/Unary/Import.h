#pragma once
#include "Unary.h"
#include <iostream>

class Import : public Unary
{
private:
    void match(std::istream &in, char c) const;
    void consumeWhite(std::istream &in) const;

public:
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override;
};