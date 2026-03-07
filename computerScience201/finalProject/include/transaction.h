#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>
#include "date.h"
// FIXME: does transaction only mark when a book was taken, received or both??

class Transaction {
    int patronId;
    std::string bookTitle;
    Date date;
    std::string inOrOut; // this will be either "CheckedOut" or "Returned"
    
    public:
        Transaction(int pid, std::string bt, Date d, std::string inOrOut);
        void displayTransaction() const;
};


#endif