#pragma once
#include <iostream>
#include <string>

class Book {
private:
	std::string title_;
	std::string author_;
public:
	Book(std::string title, std::string author)
		: title_(std::move(title)), author_(std::move(author)) {
		if (title_.empty() || author_.empty()) {
			throw std::invalid_argument("Book title and author cannot be empty.");
		}
	}

	const std::string& getTitle() const noexcept { return title_; }
	const std::string& getAuthor() const noexcept { return author_; }
};