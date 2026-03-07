#include "book.h"
#include <iostream>

Book::Book(std::string t, std::string a, Genre g) : title(t), author(a), genre(g) {}

void Book::displayInfo() const {
    std::cout << title << " by " << author << std::endl;
    std::cout << static_cast<int>(genre) << " " << static_cast<int>(status) << std::endl;
}

void EBook::displayInfo() const {
    Book::displayInfo();
    std::cout << "Book download size: " << fileSizeMB << std::endl;
}

void PrintedBook::displayInfo() const {
    Book::displayInfo();
    std::cout << "Pages: " << pageCount << std::endl;
}

void Book::setStatus(BookStatus s) {
    status = s;
}

BookStatus Book::getStatus() const {
    return status;
}

Genre Book::stringToGenre(const std::string& genreString) {
    if (genreString == "Fiction") return Genre::Fiction;
    if (genreString == "NonFiction") return Genre::NonFiction;
    if (genreString == "Mystery") return Genre::Mystery;
    if (genreString == "Science") return Genre::Science;
    if (genreString == "Biography") return Genre::Biography;
    return Genre::Fiction;
}

std::string Book::getGenre() const {
    switch(genre) {
        case Genre::Fiction: return "Fiction";
        case Genre::NonFiction: return "NonFiction";
        case Genre::Mystery: return "Mystery";
        case Genre::Science: return "Science";
        case Genre::Biography: return "Biography";
    }
    return "Unknown";
}

std::string Book::getTitle() const {
    return title;
}

std::string Book::getAuthor() const {
    return author;
}

std::string EBook::getType() const {
    return "EBook";
}

std::string EBook::getValue() const {
    return std::to_string(fileSizeMB);
}

std::string PrintedBook::getType() const {
    return "Printed";
}

std::string PrintedBook::getValue() const {
    return std::to_string(pageCount);
}

std::string Book::getType() const {
    return "Book";
}

std::string Book::getValue() const {
    return "";
}

bool Book::operator==(const Book& other) const {
    return title == other.title && author == other.author && genre == other.genre;
}

std::ostream& operator<<(std::ostream& out, const Book& book) {
    out << book.getGenre() << "," << book.getTitle() << "," << book.getAuthor() << "," << book.getType() << "," << book.getValue() << std::endl;
    return out;
}

EBook::EBook(std::string t, std::string a, Genre g, double size) : Book(t, a, g), fileSizeMB(size) {
    // set the values and do nothing
}

PrintedBook::PrintedBook(std::string t, std::string a, Genre g, int pages) : Book(t, a, g), pageCount(pages) {
    // set the values and do nothing
}