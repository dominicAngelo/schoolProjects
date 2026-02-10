#include "book.h"

Book::Book(std::string t, std::string a, Genre g) : title(t), author(a), genre(g) {}

void Book::displayInfo() const {} // TODO: implement the displayInfo functions
void EBook::displayInfo() const {}
void PrintedBook::displayInfo() const {}

void Book::setStatus(BookStatus s) {
    status = s;
}

BookStatus Book::getStatus() const {
    return status;
}

// TODO: implement operator overload functions

EBook::EBook(std::string t, std::string a, Genre g, double size) : Book(t, a, g), fileSizeMB(size) {}

PrintedBook::PrintedBook(std::string t, std::string a, Genre g, int pages) : Book(t, a, g), pageCount(pages) {}