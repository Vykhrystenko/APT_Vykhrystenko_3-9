#pragma once

#include <iostream>
#include <string>
#include <utility>

class ScopeLogger {
private:
    std::string name_;

public:
    explicit ScopeLogger(std::string name)
        : name_(std::move(name))
    {
        std::cout << "[ENTER] " << name_ << '\n';
    }

    ~ScopeLogger() noexcept
    {
        try {
            std::cout << "[EXIT] " << name_ << '\n';
        }
        catch (...) {
            // Деструктор не повинен випускати винятки
        }
    }

    ScopeLogger(const ScopeLogger&) = delete;

    ScopeLogger& operator=(const ScopeLogger&) = delete;
};