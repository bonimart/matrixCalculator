#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <memory>
#include <list>
#include <set>
#include "SparseMatrix.h"
#include "DenseMatrix.h"

class Calculator
{
private:
    std::unordered_map<std::string, std::shared_ptr<Matrix>> variables;

public:
    void addMatrix(const Matrix &m);
    void importMatrix(std::ifstream &ifs);
    void exportMatrix(std::ofstream &ofs) const;

    void parse(std::istream &is) const;
    void execute(std::istringstream &iss);

    Matrix &inverse(const Matrix &m) const;
    Matrix &merge(const Matrix &m1, const Matrix &m2) const;

    Matrix &gem(const Matrix &m, bool verbose = false) const;
    std::list<Matrix &> factLU(const Matrix &m) const;
    std::list<Matrix &> factQR(const Matrix &m) const;
    std::list<Matrix &> factSVD(const Matrix &m) const;
    std::set<double> eigenvalues(const Matrix &m) const;
};