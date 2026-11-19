#include <iostream>
#include "include/book.h"
#include "include/library.h"
#include "include/IBookRepository.h"        
#include "include/INotificationSender.h"    

// Додаткові тестові реалізації для демонстрації можливості заміни без зміни Library
class FileBookRepository : public IBookRepository {
public:
    void save(const Book& book) override {
        std::cout << "Saving book \"" << book.getTitle() << "\" to local file..." << std::endl;
    }
};

class SmsNotificationSender : public INotificationSender {
public:
    void notifyNewBook(const Book& book) override {
        std::cout << "Sending SMS notification for new book: "
            << book.getTitle() << " by " << book.getAuthor() << std::endl;
    }
};

int main() {
    // Робота зі стандартними компонентами
    DatabaseBookRepository dbRepository;
    EmailNotificationSender emailSender;

    Library library(dbRepository, emailSender);

    Book book1("The Catcher in the Rye", "J.D. Salinger");
    Book book2("To Kill a Mockingbird", "Harper Lee");

    library.addBook(book1);
    library.addBook(book2);

    std::cout << "\n--- Books in primary library ---\n";
    library.displayBooks();

    // Зміна сховища та каналу сповіщень без зміни Library
    std::cout << "\n--- Switching to File Repository & SMS Sender ---\n";
    FileBookRepository fileRepository;
    SmsNotificationSender smsSender;

    Library alternativeLibrary(fileRepository, smsSender);
    alternativeLibrary.addBook(Book("1984", "George Orwell"));
    alternativeLibrary.displayBooks();

    return 0;
}