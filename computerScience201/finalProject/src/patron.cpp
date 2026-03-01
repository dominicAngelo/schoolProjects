#include "../include/patron.h"

Patron::Patron(std::string n, int i) : name(n), id(i) {
    // set the values and do nothing
}

int Patron::getId() const {
    return id;
}

std::string Patron::getName() const {
    return name;
}

void Patron::borrowBook(Book *b) {} // TODO implement borrowBook()

void Patron::returnBook(Book *b) {} // TODO implement returnBook()

void Patron::displayPatron() const {} // TODO implement displayPatron()

// TODO implement operator overloads