#pragma once
#include <iostream>
#include "book.h"

class DisplayBook {
public:
    void display(const Book& book) const {
        std::cout << "Title: " << book.getTitle()
            << ", Author: " << book.getAuthor() << std::endl;
    };
};