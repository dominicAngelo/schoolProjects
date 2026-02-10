#include "transaction.h"

Transaction::Transaction(int pid, std::string bt) : patronId(pid), bookTitle(bt) {}

void Transaction::displayTransaction() const {} // TODO implement displayTransaction()