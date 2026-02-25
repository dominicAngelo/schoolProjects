#ifndef PATRON_H
#define PATRON_H

#include <string>
#include <vector>
#include "book.h"

class Patron {
    std::string name;
    int id;
    std::vector<Book*> borrowedBooks;
    
    public:
        Patron(std::string n, int i);
        void borrowBook(Book *b);
        void returnBook(Book *b);
        void displayPatron() const;

        bool operator==(const Patron& other) const;
        friend std::ostream& operator<<(std::ostream&, const Patron &p);
};

#endif