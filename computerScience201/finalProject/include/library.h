#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <vector>
#include "book.h"
#include "patron.h"
#include "transaction.h"

class Library {
    std::vector<Book*> books;
    std::vector<Patron> patrons;
    std::vector<Transaction> transactions;
    static int booksRead;
    static int patronsRead;
    
    public:
        ~Library();
        void loadData();
        void saveData();
        void loadBooks(int &booksRead);
        void loadPatrons(int &patronsRead);
        void addBook(Book *b);
        void addPatron(const Patron &p);
        void checkoutBook(int patronId, std::string title);
        void returnBook(int patronId, std::string title);
        void displayBooks() const;
        void displayPatrons() const;
};

#endif