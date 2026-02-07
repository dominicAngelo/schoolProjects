#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>

class Transaction {
    int patronId;
    std::string bookTitle;
    std::string date;
    
    public:
        Transaction(int pid, std::string bt);
        void displayTransaction() const;
};


#endif