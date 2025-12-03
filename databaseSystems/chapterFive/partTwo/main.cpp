/* Program name: main.cpp
   Author: Dominic Angelo
   Date last updated: 11/28/25
   Purpose: Open and read from sakila database
*/

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <sqlite3.h>    

using namespace std;

//this is a big file so using std namespace to reduce amount of typing

struct customerMenu {
    int id;
    string firstName;
    string lastName;
};

struct rentalMenu {
    int id;
    string rentalDate;
};


vector<customerMenu> allCustomers;
vector<rentalMenu> allRentals;
int currentCustomerID = -1;

int customerCallback(void *data, int argc, char** argv, char** columnNames) {
    customerMenu customer;

    if (argv[0] != NULL && argv[1] != NULL && argv[2] != NULL) {
        customer.id = atoi(argv[0]);
        customer.firstName = argv[1];
        customer.lastName = argv[2];
        allCustomers.push_back(customer);
    }
    return 0;
}

int rentalCallback(void *data, int argc, char** argv, char** columnNames) {
    rentalMenu rental;

    if (argv[0] != NULL && argv[1] != NULL) {
        rental.id = atoi(argv[0]);
        rental.rentalDate = argv[1];
        allRentals.push_back(rental);
    }
    return 0;
}

void loadCustomers(sqlite3 *db) {
    if (allCustomers.empty()) {
        const char *getCustomersCommand = 
            "SELECT "
            "customer_id, "
            "first_name, " 
            "last_name " 
            //"email, "
            //"address_id, "
            //"last_update "
            "FROM "
            "customer "
            "ORDER BY "
            "customer_id;";

        char *errorMsg = nullptr;
        int runQuery = sqlite3_exec(db, getCustomersCommand, customerCallback, nullptr, &errorMsg);

        if (runQuery != SQLITE_OK) {
            cerr << "Error with loading customers" << errorMsg << endl;
        }
    }
}

void loadRentals(sqlite3 *db, int customerID) {
    allRentals.clear();

    const string getRentalsCommandString = 
        "SELECT "
        "rental_id, "
        "rental_date "
        "FROM rental "
        "WHERE customer_id = " + to_string(customerID) +   // huge sql injection risk here, but assuming for schoolwork it is just fine
        " ORDER BY rental_id;";

    const char *getRentalsCommand = getRentalsCommandString.c_str();
    char *errorMsg = nullptr;

    int runQuery = sqlite3_exec(db, getRentalsCommand, rentalCallback, nullptr, &errorMsg);
    
    if (runQuery != SQLITE_OK) {
        cerr << "Error with loading rentals" << errorMsg << endl;
    }
}

void displayCustomer(int customerID) {
    sqlite3 *db;
    sqlite3_stmt *statement;

    int sakilaCustomers = sqlite3_open("sakila.db", &db);

    if (sakilaCustomers != SQLITE_OK) {
        cerr << "Error opening sakila database" << endl;
    }

    const char *getCustomerCommand = 
        "SELECT "
        "cust.customer_id, cust.first_name, cust.last_name, "
        "a.address, cit.city, a.phone, cust.email, cust.last_update "
        "FROM customer cust "
        "JOIN address a ON cust.address_id = a.address_id "
        "JOIN city cit ON a.city_id = cit.city_id "
        "WHERE cust.customer_id = ?;";

    sakilaCustomers = sqlite3_prepare_v2(db, getCustomerCommand, -1, &statement, NULL);

    if (sakilaCustomers != SQLITE_OK) {
        cerr << "Error with loading customers" << endl;
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(statement, 1, customerID);

    //sakilaCustomers = sqlite3_step(statement);

    if (sqlite3_step(statement) == SQLITE_ROW) {
        string firstName = (const char*)sqlite3_column_text(statement, 1);
        string lastName = (const char*)sqlite3_column_text(statement, 2);
        string address = (const char*)sqlite3_column_text(statement, 3);
        string city = (const char*)sqlite3_column_text(statement, 4);
        string phone = (const char*)sqlite3_column_text(statement, 5);
        string email = (const char*)sqlite3_column_text(statement, 6);
        string lastUpdate = (const char*)sqlite3_column_text(statement, 7);

        cout << "----Customer Information----" << endl;
        cout << "Name: " << firstName << " " << lastName << endl;
        cout << "Address: " << address << endl;
        cout << "City: " << city << endl;
        cout << "Phone Number: " << phone << endl;
        cout << "Email: " << email << endl;
        cout << "Last Update: " << lastUpdate << endl; 
    } else {
        cerr << "Customer not found" << endl;
    }

    sqlite3_finalize(statement);
    sqlite3_close(db);
}

void displayRental(int rentalID) {
    sqlite3 *db;
    sqlite3_stmt *statement;
    int sakilaRentals = sqlite3_open("sakila.db", &db);

    if (sakilaRentals != SQLITE_OK) {
        cerr << "Error with rentals" << endl;
    }

    const char *getRentalsCommand = 
        "SELECT "
        "r.rental_date, s.first_name, s.last_name, " 
        "c.first_name, c.last_name, " 
        "f.title, f.description, f.rental_rate, r.return_date " 
        "FROM rental r "
        "JOIN inventory i ON r.inventory_id = i.inventory_id "
        "JOIN film f ON i.film_id = f.film_id "
        "JOIN staff s ON r.staff_id = s.staff_id "
        "JOIN customer c ON r.customer_id = c.customer_id "
        "WHERE r.rental_id = ?;";

    sakilaRentals = sqlite3_prepare_v2(db, getRentalsCommand, -1, &statement, NULL);

    if (sakilaRentals != SQLITE_OK) {
        cerr << "Error with loading rentals" << endl;
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(statement, 1, rentalID);

    if (sqlite3_step(statement) == SQLITE_ROW) {
        string rentalDate = (const char*)sqlite3_column_text(statement, 0);
        string staffFirstName = (const char*)sqlite3_column_text(statement, 1);
        string staffLastName = (const char*)sqlite3_column_text(statement, 2);
        string firstName = (const char*)sqlite3_column_text(statement, 3);
        string lastName = (const char*)sqlite3_column_text(statement, 4);
        string title = (const char*)sqlite3_column_text(statement, 5);
        string description = (const char*)sqlite3_column_text(statement, 6);
        double rentalRate = sqlite3_column_double(statement, 7);
        const char* returnDateString = (const char*)sqlite3_column_text(statement, 8);

        if (returnDateString == NULL) {
            returnDateString = "N/A";
        }

        cout << "Rental Date " << rentalDate << endl;
        cout << "Staff: " << staffFirstName  << " " << staffLastName << endl;
        cout << "Customer: " << firstName << " " << lastName << endl;
        cout << "Film Information:" << endl;

        cout << fixed << setprecision(2);
        cout << title << "-" << description << " $" << rentalRate << endl;
    } else {
        cerr << "Error with SQL getRentals";
    }

    sqlite3_finalize(statement);
    sqlite3_close(db);
}

void clearInput() {     // using this to reduce typing
    if (cin.fail()) {
        cin.clear();
        cin.ignore(100000, '\n');
    }
}

int displayCustomerMenu(sqlite3 *db) {

    loadCustomers(db);

    int totalRows = allCustomers.size();
    if (totalRows == 0) {
        cout << "No customers found" << endl;
        return 1;
    }

    int rowsPerPage;
    while (true) {
        cout << "There are " << totalRows << " rows in the result. How many do you want to see per page?" << endl;  // FIXME: ISSUE WITH TOTAL ROWS NOT DISPLAYING CORRECT NUMBER
        cout << "Enter choice: ";
        if (!(cin >> rowsPerPage)) {
            cerr << "Invalid input. Please enter a number" << endl;
            clearInput();
            continue;
        }
        if (rowsPerPage <= 0) {
            cout << "Rows per page must be a positive number. Defaulting to 10" << endl;
            rowsPerPage = 10;
        }
        cout << rowsPerPage << endl;
        break;
    }

    int page = 0;

    while (true) {
        int start = page * rowsPerPage;
        int end = min(start + rowsPerPage, totalRows);
        int currentOption = start + 1;

        cout << "Please choose the customer you want to see rentals for (enter 0 to go to the next page)";
        if (page > 0) {
            cout << " or -1 to go to the previous page";
        }
        cout << "):" << endl;

        for (int i = start; i < end; ++i) {
            cout << currentOption++ << ". ";
            cout << allCustomers[i].id << "- ";
            cout << allCustomers[i].firstName << " ";
            cout << allCustomers[i].lastName << endl;
        }

        int menuChoice;
        cout << "Enter Choice: ";

        if (!(cin >> menuChoice)) {
            cerr << "Invalid input. Please enter a number." << endl;
            clearInput();
            continue;
        }
        
        if (menuChoice == 0) {
            if (end < totalRows) { 
                page++; 
            } else { 
                cout << "You are on the last page." << endl; 
            }
            continue;
        } else if (menuChoice == -1) {
            if (page > 0) { 
                page--; 
            } else { 
                cout << "You are on the first page." << endl; 
            }
            continue;
        };
        if (menuChoice >= start + 1 && menuChoice <= end) {
            int index = menuChoice - 1;
            return allCustomers[index].id;
        } else {
            cerr << "Please choose a valid customer id";
        }
    }  
}

int displayRentalMenu(sqlite3 *db, int customerID) {
    loadRentals(db, customerID);
    int totalRows = allRentals.size();
    if (totalRows == 0) {
        cout << "No rentals found for this customer" << endl;
        return 1;
    }
    int rowsPerPage;

     while (true) {
        cout << "There are " << totalRows << " rows in the result. How many do you want to see per page?" << endl;
        cout << "Enter choice: ";
        if (!(cin >> rowsPerPage)) {
            cerr << "Invalid input. Please enter a number" << endl;
            clearInput();
            continue;
        }
        if (rowsPerPage <= 0) {
            cout << "Rows per page must be a positive number. Defaulting to 10" << endl;
            rowsPerPage = 10;
        }
        cout << rowsPerPage << endl;
        break;
    }

    int page = 0;
    
    while (true) {
        int start = page * rowsPerPage;
        int end = min(start + rowsPerPage, totalRows);
        int currentOption = start + 1;

        cout << "Please choose the rental you want to see (enter 0 to go to the next page)";
        if (page > 0) {
            cout << " or -1 to go to the previous page";
        }
        cout << "):" << endl;

        for (int i = start; i < end; ++i) {
            cout << currentOption++ << ". ";
            cout << allRentals[i].id << "- ";
            cout << allRentals[i].rentalDate << endl;
        }

        int menuChoice;
        cout << "Enter Choice: ";

        if (!(cin >> menuChoice)) {
            cerr << "Invalid input. Please enter a number." << endl;
            clearInput();
            continue;
        }
        
        if (menuChoice == 0) {
            if (end < totalRows) { 
                page++; 
            } else { 
                cout << "You are on the last page." << endl; 
            }
            continue;
        } else if (menuChoice == -1) {
            if (page > 0) { 
                page--; 
            } else { 
                cout << "You are on the first page." << endl; 
            }
            continue;
        }

        if (menuChoice >= start + 1 && menuChoice <= end) {
            int index = menuChoice - 1;
            return allRentals[index].id;
        } else {
            cerr << "Please choose a valid rental id";
        }
    }  
}

void viewCustomer() {
    sqlite3 *db;
    int status = sqlite3_open("sakila.db", &db);

    if (status != SQLITE_OK) {
        cerr << "Error with opening database" << endl;
        return;
    }

    cout << "Please choose the customer you want to see: " << endl;
    int selectedID = displayCustomerMenu(db);

    sqlite3_close(db);

    if (selectedID > 0) {
        displayCustomer(selectedID);
    }
}

void rentalQuery() {
    sqlite3 *db;
    int rental = sqlite3_open("sakila.db", &db);

    if (rental != SQLITE_OK) {
        cerr << "Error opening database" << endl;
        return;
    }

    int customerID = displayCustomerMenu(db);

    if (customerID > 0) {
        int rentalID = displayRentalMenu(db, customerID);
        
        if (rentalID > 0) {
            sqlite3_close(db);
            displayRental(rentalID);
            return;
        }
    }
    sqlite3_close(db);
}

int main() {
    int choice;

    enterChoice:

    cout << "Welcome to Sakila" << endl;
    cout << "Please choose an option (enter -1 to quit) :" << endl;
    cout << "1. View the rentals for a customer" << endl;
    cout << "2. View Customer Information" << endl;
    cout << "Enter choice: ";
    
    cin >> choice;

    if (choice == -1) {
        exit(0);
    }

    if (choice == 1) {
        rentalQuery();
    } else if (choice == 2) {
        viewCustomer();
    } else {
        cout << "Please enter a valid number 1, 2, or -1" << endl;
    }

    cout << endl;
    goto enterChoice;

    return 0;
}