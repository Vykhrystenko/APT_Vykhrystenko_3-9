#pragma once
#include "book.h"
#include <iostream>

class INotificationSender {
public:
    virtual void notifyNewBook(const Book& book) = 0;
    virtual ~INotificationSender() = default;
};

class EmailNotificationSender : public INotificationSender {
public:
	void notifyNewBook(const Book& book) override {
		std::cout << "Sending email notification for new book: "
			<< book.getTitle() << " by " << book.getAuthor() << std::endl;
	}
};