#include "../include/patron.h"
#include <iostream>
#include <vector>
#include "../include/book.h"
#include "../include/transaction.h"

Patron::Patron(std::string n, int i) : name(n), id(i) {
    // set the values and do nothing
}

int Patron::getId() const {
    return id;
}

std::string Patron::getName() const {
    return name;
}

void Patron::borrowBook(Book *b, std::vector<Book*> &libraryBooks, std::vector<Transaction> &transactions) { //we will use the time to make a new transaction,  
    std::cout << "Please enter the title of the book to borrow: ";   //we will have to convert ctime current time to our current time class and use that when creating a transaction
    std::string title;                                             
    std::getline(std::cin, title);
    for (int i = 0; i < libraryBooks.size(); i++) {

        if (libraryBooks[i]->getTitle() == title) {
            Book *bookToBorrow = libraryBooks[i];

            if (bookToBorrow->getStatus() == BookStatus::Available) {
                bookToBorrow->setStatus(BookStatus::CheckedOut);
                borrowedBooks.push_back(bookToBorrow);

                Transaction transaction(id, title, Date::getCurrentDate(), "Out");
                transactions.push_back(transaction);
                std::cout << "You have borrowed the book: " << title << std::endl;
                return;
            } else {
                std::cout << "Sorry, the book is currently not available." << std::endl;
                return;
            }
        }
    }

    std::cout << "The book you requested is not available in the library." << std::endl;
}

void Patron::returnBook(Book *b, std::vector<Book*> &borrowedBooks, std::vector<Transaction> &transactions) {
    std::cout << "Please enter the title of the book to return: ";
    std::string title;
    std::getline(std::cin, title);

    for (int i = 0; i < this->borrowedBooks.size(); i++) {
        if (this->borrowedBooks[i]->getTitle() == title) {
            Book *bookToReturn = this->borrowedBooks[i];
            bookToReturn->setStatus(BookStatus::Available);
            this->borrowedBooks.erase(this->borrowedBooks.begin() + i);
            Transaction transaction(id, title, Date::getCurrentDate(), "In");
            transactions.push_back(transaction);
            std::cout << "You have returned the book: " << title << std::endl;
            return;
        }
    }
    std::cout << "The book you requested is not in your borrowed list." << std::endl;
}

void Patron::displayPatron() const {
    std::cout << "Patron ID: " << id << ", Name: " << name << std::endl;
    std::cout << "Borrowed Books: " << std::endl;
    for (int i = 0; i < borrowedBooks.size(); i++) {
        std::cout << " -" << borrowedBooks[i]->getTitle() << " by " << borrowedBooks[i]->getAuthor() << std::endl;
    }
}

bool Patron::operator==(const Patron& other) const {
    return id == other.id && name == other.name;
}

std::ostream& operator<<(std::ostream& out, const Patron &patron) {
    out << patron.getId() << "," << patron.getName() << std::endl;
    return out;
}