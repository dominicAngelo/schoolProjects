#include "transaction.h"
#include <iostream>

Transaction::Transaction(int pid, std::string bt, Date d) : patronId(pid), bookTitle(bt), date(d) {
    // set the values and do nothing
}

void Transaction::displayTransaction() const {
    std::cout << "Patron: " << patronId << std::endl;
    std::cout << bookTitle << std::endl;
    std::cout << date.toString() << std::endl; 
    // does this need to be specific about who gave/received the book 
    // in the particular transaction??
}