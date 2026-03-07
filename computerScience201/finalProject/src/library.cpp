#include "library.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int Library::booksRead = 0;
int Library::patronsRead = 0;

void Library::loadBooks(int &booksRead) {
    std::ifstream booksFile("books.txt");

    if (!booksFile.is_open()) {
        std::cerr << "!!! ERROR !!! books.txt could not be found on line 16" << std::endl;
        return;
    }

    std::string line;
    std::string genre, title, author, type, value;

    while (std::getline(booksFile, line)) {

        if (line.empty()) {
            continue;
        }  
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
}

void Library::loadPatrons(int &patronsRead) {
    std::ifstream patronsFile("patrons.txt");

    if (!patronsFile.is_open()) {
        std::cerr << "!!! ERROR !!! could not open patrons.txt on line 55" << std::endl;
        return;
    }

    std::string line;
    std::string id, name;
    while (std::getline(patronsFile, line)) {

        if (line.empty()) {
            continue;
        }
        std::stringstream patronLine(line);
        std::getline(patronLine, id, ',');
        std::getline(patronLine, name, ',');
        Patron patron(name, std::stoi(id));
        patrons.push_back(patron);
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

void Library::saveData() {
    std::ofstream booksFile("books.txt");
    std::ofstream patronsFile("patrons.txt");

    if (!booksFile.is_open()) {
        std::cerr << "!!! ERROR !!! could not open books.txt in function saveData()" << std::endl;
        return;
    }

    for (int i = 0; i < books.size(); i++) {
        booksFile << books.at(i)->getGenre() << "," << books.at(i)->getTitle() << "," << books.at(i)->getAuthor() << "," << books.at(i)->getType() << "," << books.at(i)->getValue() << std::endl;
    }

    booksFile.close();

    if (!patronsFile.is_open()) {
        std::cerr << "!!! ERROR !!! could not open patrons.txt in function saveData()" << std::endl;
        return;
    }

    for (int i = 0; i < patrons.size(); i++) {
        patronsFile << patrons.at(i).getId() << "," << patrons.at(i).getName() << std::endl;
    }
    patronsFile.close();
}

void Library::addPatron(const Patron &p) {
    std::string name;
    int id = patrons.size() + 1;

    std::cout << "Enter the patron's name: " << std::endl;
    std::getline(std::cin, name);
    Patron patron(name, id);
    patrons.push_back(patron);
}

void Library::checkoutBook(int patronId, std::string title) {
    for (int i = 0; i < patrons.size(); i++) {
        if (patrons.at(i).getId() == patronId) {
            patrons.at(i).borrowBook(nullptr, books);
            return;
        }
    }
    std::cout << "Patron not found." << std::endl;
}

void Library::returnBook(int patronId, std::string title) {
    for (int i = 0; i < patrons.size(); i++) {
        if (patrons.at(i).getId() == patronId) {
            patrons.at(i).returnBook(nullptr, books);
            return;
        }
    }
    std::cout << "Patron not found." << std::endl;
}

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

Library::~Library() {
    for (Book* book : books) {
        delete book;
    }
    books.clear();
}