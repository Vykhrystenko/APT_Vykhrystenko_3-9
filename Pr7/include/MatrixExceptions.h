#pragma once

#include <stdexcept>
#include <string>

// Базовий клас винятків бібліотеки
class MatrixException : public std::runtime_error {
public:
    explicit MatrixException(const std::string& message)
        : std::runtime_error(message)
    {
    }
};

// Некоректні вхідні дані або нульовий вказівник
class InvalidMatrixException : public MatrixException {
private:
    std::string parameter_;

public:
    InvalidMatrixException(
        const std::string& parameter,
        const std::string& message)
        : MatrixException(message),
        parameter_(parameter)
    {
    }

    const std::string& parameter() const noexcept {
        return parameter_;
    }
};

// Несумісні розміри матриць або вектора
class MatrixDimensionException : public MatrixException {
private:
    std::string operation_;

public:
    MatrixDimensionException(
        const std::string& operation,
        const std::string& message)
        : MatrixException(message),
        operation_(operation)
    {
    }

    const std::string& operation() const noexcept {
        return operation_;
    }
};