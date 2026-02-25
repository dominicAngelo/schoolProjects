#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>
// FIXME: does transaction only mark when a book was taken, received or both??

class Transaction {
    int patronId;
    std::string bookTitle;
    std::string date;
    
    public:
        Transaction(int pid, std::string bt);
        void displayTransaction() const;
};


#endif