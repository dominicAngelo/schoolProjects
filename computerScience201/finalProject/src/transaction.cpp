#include "transaction.h"
#include <iostream>
#include <ctime>

Transaction::Transaction(int pid, std::string bt, Date d, std::string inOrOut) : patronId(pid), bookTitle(bt), date(d), inOrOut(inOrOut) {
    // set the values and do nothing
}

void Transaction::displayTransaction() const {
    std::cout << "Patron: " << patronId << std::endl;
    std::cout << bookTitle << std::endl;
    std::cout << date.toString() << std::endl;
    std::cout << "Transaction Type: " << inOrOut << std::endl;
}