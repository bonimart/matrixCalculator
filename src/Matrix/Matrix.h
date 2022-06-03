#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include "../Index.h"
#include "../settings.h"

// TODO dodelat print u dense a sparse
class Matrix
{
private:
    class Data
    {
    public:
        virtual double at(std::size_t i, std::size_t j) const = 0;
        virtual void set(std::size_t i, std::size_t j, const double val) = 0;
        virtual void print(std::ostream &out, std::size_t shape_y, std::size_t shape_x) const = 0;
        virtual std::shared_ptr<Data> clone() const = 0;
    };
    class DataDense : public Data
    {
    protected:
        std::vector<std::vector<double>> m_data;

    public:
        DataDense(const std::vector<std::vector<double>> &data)
            : m_data(data) {}
        DataDense(const std::shared_ptr<Data> data, std::size_t shape_y, std::size_t shape_x);
        virtual double at(std::size_t i, std::size_t j) const override;
        virtual void set(std::size_t i, std::size_t j, const double val) override;
        virtual void print(std::ostream &out, std::size_t shape_y, std::size_t shape_x) const override;
        virtual std::shared_ptr<Data> clone() const override;
    };
    class DataSparse : public Data
    {
    protected:
        std::unordered_map<std::size_t, std::unordered_map<std::size_t, double>> m_data;

    public:
        DataSparse(const std::unordered_map<std::size_t, std::unordered_map<std::size_t, double>> &data)
            : m_data(data) {}
        DataSparse(const std::vector<std::vector<double>> &data);
        DataSparse(const std::shared_ptr<Data> data, std::size_t shape_y, std::size_t shape_x);
        virtual double at(std::size_t i, std::size_t j) const override;
        virtual void set(std::size_t i, std::size_t j, const double val) override;
        virtual void print(std::ostream &out, std::size_t shape_y, std::size_t shape_x) const override;
        virtual std::shared_ptr<Data> clone() const override;
    };

    int lastZeroCount = -1;
    int zeroCount = 0;

    std::shared_ptr<Data> m_data;

public:
    std::size_t m_shape_y = 0;
    std::size_t m_shape_x = 0;

    Matrix(const std::vector<std::vector<double>> &data);
    Matrix(const Matrix &other);
    Matrix(const std::size_t shape_y, const std::size_t shape_x, const double fill = 0);
    ~Matrix() = default;

    Matrix &operator=(const Matrix &other);
    Matrix operator+(const double val) const;
    Matrix operator*(const double val) const;

    double at(std::size_t i, std::size_t j) const;
    void set(std::size_t i, std::size_t j, double val);
    void balance();

    void print(std::ostream &out) const;
};