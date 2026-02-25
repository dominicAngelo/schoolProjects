#include "library.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

// TODO implement library constructor if needed

std::vector<Book*> books;
int booksRead = 0;

std::ifstream patronsFile("patrons.txt");

Book readBook(int &booksRead, Library &library) {
    std::ifstream booksFile("books.txt");

    if (!booksFile.is_open()) {
        std::cerr << "!!! ERROR !!! books.txt could not be found" << std::endl;
    }

    std::string line;
    std::string genre, title, author, type, value;
    int counter = 0;

    while (std::getline(booksFile, line)) {

        if (line.empty()) {
            continue;
        } else if (counter == (booksRead + 1)) {
            std::stringstream bookLine(line);
            std::getline(bookLine, genre, ',');
            std::getline(bookLine, title, ',');
            std::getline(bookLine, author, ',');
            std::getline(bookLine, type, ',');
            std::getline(bookLine, value, ',');

            booksRead++;

            if (type == "EBook") {
                Book* eBook = new EBook(title, author, Book::stringToGenre(genre), std::stod(value));
                library.addBook(eBook);
            } else if (type == "Printed") {
                Book* pBook = new PrintedBook(title, author, Book::stringToGenre(genre), std::stoi(value));
                library.addBook(pBook);
            }
        }
        counter++;
    }
}

void Library::addBook(Book *b) {
    books.push_back(b);
}

void Library::loadData() {} // TODO implement loadData()

void Library::saveData() {} // TODO implement saveData()

void Library::addPatron(const Patron &p) {} // TODO implement addPatron()

void Library::checkoutBook(int patronId, std::string title) {} // TODO implement checkoutBook()

void Library::returnBook(int patronId, std::string title) {} // TODO implement returnBook()

void Library::displayBooks() const {} // TODO implement displayBooks()

void Library::displayPatrons() const {} // TODO implement displayPatrons()