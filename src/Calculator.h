#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <memory>
#include <list>
#include <set>
#include "Matrix/Matrix.h"
#include "Operations/Operation.h"

class Calculator
{
private:
    std::unordered_map<std::string, std::shared_ptr<Matrix>> variables;
    std::unordered_map<std::string, std::unique_ptr<Operation>> operations;

public:
    void addMatrix(const Matrix &m);
    void importMatrix(std::ifstream &ifs);
    void exportMatrix(std::ofstream &ofs) const;

    // zparsuje jeden radek
    void parse(std::istream &is) const;
    void execute(std::istringstream &iss);
};