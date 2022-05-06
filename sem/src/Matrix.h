#include <iostream>
#include "Point.h"

class Matrix
{
protected:
    Point m_shape;
    int m_det;
    int m_rank;

public:
    Matrix(const int x, const int y);
    Matrix(const Matrix &other);
    virtual ~Matrix() = default;

    virtual Matrix &operator=(const Matrix &other);
    Matrix &operator+(const Matrix &other) const;
    Matrix &operator-(const Matrix &other) const;
    Matrix &operator*(const Matrix &other) const;

    virtual Matrix &transpose() const = 0;
    virtual Matrix &crop(const Point &shape) const = 0;

    virtual void print(std::ostream &os) const = 0;
    virtual double &get(const Point &coords) const = 0;

    int det() const;
    int rank() const;
};