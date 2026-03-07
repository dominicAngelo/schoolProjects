#ifndef PATRON_H
#define PATRON_H

#include <string>
#include <vector>
#include "book.h"
#include "transaction.h"

class Patron {
    std::string name;
    int id;
    std::vector<Book*> borrowedBooks;
    std::vector<Transaction> transactions;
    
    public:
        Patron(std::string n, int i);
        void borrowBook(Book *b, std::vector<Book*> &libraryBooks, std::vector<Transaction> &transactions);
        void returnBook(Book *b, std::vector<Book*> &libraryBooks, std::vector<Transaction> &transactions);
        void displayPatron() const;

        int getId() const;
        std::string getName() const;

        bool operator==(const Patron& other) const;
        friend std::ostream& operator<<(std::ostream&, const Patron &p);
};

#endif