#include "transaction.h"
#include <iostream>

Transaction::Transaction(int pid, std::string bt) : patronId(pid), bookTitle(bt) {
    // set the values and do nothing
}

void Transaction::displayTransaction() const {
    std::cout << "Patron: " << patronId << std::endl;
    std::cout << bookTitle << std::endl;
    std::cout << date << std::endl; 
    // does this need to be specific about who gave/received the book 
    // in the particular transaction??
}