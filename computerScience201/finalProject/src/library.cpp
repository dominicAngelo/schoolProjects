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
    // TODO implement user input to take input
}

void Library::loadData() {
    loadBooks(booksRead);
    loadPatrons(patronsRead);
}

void Library::saveData() {} // TODO implement saveData()

void Library::addPatron(const Patron &p) {
  
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