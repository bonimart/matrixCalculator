#pragma once
#include <map>
#include "Unary.h"

class AddVar : public Unary
{
protected:
    std::map<std::string, std::shared_ptr<Matrix>> &variables;

public:
    AddVar(std::map<std::string, std::shared_ptr<Matrix>> &var)
        : variables(var) {}
    virtual std::unique_ptr<Matrix> evaluate(Parameters p) const override
    {
        variables[p.param_str] = std::make_shared<Matrix>(*(p.param1));
        return std::move(p.param1);
    }
};