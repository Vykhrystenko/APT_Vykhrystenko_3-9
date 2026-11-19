#pragma once 
#include "book.h"
#include <iostream>

class IBookRepository {
public:
    virtual void save(const Book& book) = 0;
    virtual ~IBookRepository() = default;
};

class DatabaseBookRepository : public IBookRepository {
public:
    void save(const Book& book) override {
        std::cout << "Saving book \"" << book.getTitle() << "\" to database..." << std::endl;
    }
};