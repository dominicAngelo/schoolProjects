#include "library.h"
#include <fstream>
#include <iostream>
#include <string>

// TODO implement library constructor if needed

std::vector<Book> books;
int booksRead;

std::ifstream booksFile("books.txt");
std::ifstream patronsFile("patrons.txt");

void Library::addBook(Book *b) {

}

void Library::loadData() {} // TODO implement loadData()

void Library::saveData() {} // TODO implement saveData()

void Library::addPatron(const Patron &p) {} // TODO implement addPatron()

void Library::checkoutBook(int patronId, std::string title) {} // TODO implement checkoutBook()

void Library::returnBook(int patronId, std::string title) {} // TODO implement returnBook()

void Library::displayBooks() const {} // TODO implement displayBooks()

void Library::displayPatrons() const {} // TODO implement displayPatrons()