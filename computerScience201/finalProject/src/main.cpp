#include <iostream>
#include "library.h"

int main() {

    Library library;
    library.loadData();
    
    int choice;
    while (true) {
        std::cout << "Library Management System" << std::endl;
        std::cout << "1. Display Books" << std::endl;
        std::cout << "2. Display Patrons" << std::endl;
        std::cout << "3. Add Book" << std::endl;
        std::cout << "4. Add Patron" << std::endl;
        std::cout << "5. Checkout Book" << std::endl;
        std::cout << "6. Return Book" << std::endl;
        std::cout << "7. Save & Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();

         
        if (choice == 1) {
            library.displayBooks();     
        } else if (choice == 2) {
            library.displayPatrons();
        } else if (choice == 3) {
            library.addBook(nullptr);
        } else if (choice == 4) {
            Patron testPatron("Test Patron", 3);
            library.addPatron(testPatron);
        } else if (choice == 5) {
            int id; std::string title;
            std::cout << "Enter patron ID: "; std::cin >> id; std::cin.ignore();
            library.checkoutBook(id, "");
        } else if (choice == 6) {
            int id; 
            std::string title;
            std::cout << "Enter patron ID: "; 
            std::cin >> id; 
            std::cin.ignore();
            library.returnBook(id, "");
        } else if (choice == 7) {
            library.saveData();
            return 0;
        }
    }
    return 0;
}