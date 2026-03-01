#include "library.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::vector<Book*> books;
std::vector<Patron> patrons;
int booksRead = 0;
int patronsRead = 0;

void Library::loadBooks(int &booksRead) {
    std::ifstream booksFile("books.txt");

    if (!booksFile.is_open()) {
        std::cerr << "!!! ERROR !!! books.txt could not be found" << std::endl;
        abort();
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
                this->books.push_back(eBook);
            } else if (type == "Printed") {
                Book* pBook = new PrintedBook(title, author, Book::stringToGenre(genre), std::stoi(value));
                this->books.push_back(pBook);
            }
        }
        counter++;
    }
}

void Library::loadPatrons(int &patronsRead) {
    std::ifstream patronsFile("patrons.txt");
    int counter;

    if (!patronsFile.is_open()) {
        std::cerr << "!!! ERROR !!! could not open patrons.txt" << std::endl;
        abort();
    }

    std::string line;
    std::string id, name;
    while (std::getline(patronsFile, line)) {

        if (line.empty()) {
            continue;
        } else if (counter == (booksRead + 1)) {
            std::stringstream bookLine(line);
            std::getline(bookLine, id, ',');
            std::getline(bookLine, name, ',');
            patronsRead++;
            Patron patron(name, std::stoi(id));
            patrons.push_back(patron);
        }
        counter++;
    }
}

void Library::addBook(Book *b) {

    std::string title, author, genre, value;
    std::string type;
    std::cout << "Enter the book title: " << std::endl;
    std::getline(std::cin, title);
    std::cout << "Enter the author" << std::endl;
    std::getline(std::cin, author);

    while (true) {
        std::cout << "Enter the genre" << std::endl;
        std::getline(std::cin, genre);
        if (genre != "Fiction" && genre != "NonFiction" && genre != "Mystery" && genre != "Science" && genre != "Biography") {
            std::cout << "Invalid genre. Please enter: Fiction, Non-Fiction, Science, or History." << std::endl;
        } else {
            break;
        }
    }

    std::cout << "Enter book type (EBook or Printed): " << std::endl;

    while (true) {
        std::getline(std::cin, type);

        if (type != "EBook" && type != "Printed") {
            std::cout << "Invalid book type. Please enter: EBook or Printed." << std::endl;
        } else {
            break;
        }
    }

    if (type == "EBook") {
        std::cout << "Enter the download size (MB): " << std::endl;
        std::getline(std::cin, value);
        Book* eBook = new EBook(title, author, Book::stringToGenre(genre), std::stod(value));
        this->books.push_back(eBook);
    } else if (type == "Printed") {
        std::cout << "Enter book size (pages): " << std::endl;
        std::getline(std::cin, value);
        Book* pBook = new PrintedBook(title, author, Book::stringToGenre(genre), std::stoi(value));
        this->books.push_back(pBook);
    }
}

void Library::loadData() {
    loadBooks(booksRead);
    loadPatrons(patronsRead);
}

void Library::saveData() {}

void Library::addPatron(const Patron &p) {
    std::string name;
    int id = patrons.size() + 1;

    std::cout << "Enter the patron's name: " << std::endl;
    std::getline(std::cin, name);
    Patron patron(name, id);
    patrons.push_back(patron);
}

void Library::checkoutBook(int patronId, std::string title) {} // TODO implement checkoutBook()

void Library::returnBook(int patronId, std::string title) {} // TODO implement returnBook()

void Library::displayBooks() const {
    for (int i = 0; i < books.size(); i++) {
        books.at(i)->displayInfo();
    }
}

void Library::displayPatrons() const {
    for (int i = 0; i < patrons.size(); i++) {
        patrons.at(i).displayPatron();
    }
}