#include "book.h"
#include <iostream>

Book::Book(std::string t, std::string a, Genre g) : title(t), author(a), genre(g) {}

/* FIXME: review understanding of how to override cpp functions
   this will matter depending on how books are referred to in the program
   if they are pointers you may need to declare the base displayInfo as 
   virtual 
*/

void Book::displayInfo() const {
    std::cout << title << " by " << author << std::endl;
    std::cout << static_cast<int>(genre) << " " << static_cast<int>(status) << std::endl;
}

void EBook::displayInfo() const {
    std::cout << "Book download size: " << fileSizeMB << std::endl;
}

void PrintedBook::displayInfo() const {
    std::cout << "Pages: " << pageCount << std::endl;
}

void Book::setStatus(BookStatus s) {
    status = s;
}

BookStatus Book::getStatus() const {
    return status;
}

// TODO: implement operator overload functions

EBook::EBook(std::string t, std::string a, Genre g, double size) : Book(t, a, g), fileSizeMB(size) {
    // set the values and do nothing
}

PrintedBook::PrintedBook(std::string t, std::string a, Genre g, int pages) : Book(t, a, g), pageCount(pages) {
    // set the values and do nothing
}