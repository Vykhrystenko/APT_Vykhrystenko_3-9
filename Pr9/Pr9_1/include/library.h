#include <iostream>
#include <string>
#include <vector>
#include <book.h>
#include <IBookRepository.h>
#include <INotificationSender.h>
#include <displayBook.h>


class Library {
private:
    IBookRepository& repository_;
    INotificationSender& notificationSender_;
    std::vector<Book> books_;
public:
    Library(IBookRepository& repository, INotificationSender& notificationSender)
        : repository_(repository), notificationSender_(notificationSender) {
    }

    void addBook(const Book& book) {
        books_.push_back(book);
        repository_.save(book);
        notificationSender_.notifyNewBook(book);
    }

    const std::vector<Book>& getBooks() const noexcept {
        return books_;
    }
	void displayBooks() const {
		DisplayBook display;
		for (const Book& book : books_) {
			display.display(book);
		}
	}
};
