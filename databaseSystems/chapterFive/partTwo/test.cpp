/* Program name: main.cpp
   Author: Dominic Angelo
   Date last updated: 11/29/25 (Refactored to use sqlite3_exec and callback pattern)
   Purpose: Open and read from the Sakila database using sqlite3_exec and callbacks to display customer lists and details, including pagination with dynamic rows per page.
*/

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm> // Required for std::min
#include <limits>    // Required for input stream manipulation
#include <iomanip>   // Required for std::fixed and std::setprecision

// CRITICAL: Must use extern "C" for C library linkage in C++
extern "C" {
    #include <sqlite3.h> 	
}

// Struct to hold customer data for the menu list
struct CustomerMenuItem {
    int id;
    std::string firstName;
    std::string lastName;
};

// Struct to hold rental data for the menu list
struct RentalMenuItem {
    int id;
    std::string rentalDate;
};

// --- GLOBAL STORAGE ---
// Store the full list of customer IDs/Names for the menu
std::vector<CustomerMenuItem> allCustomers;

// Global storage for the rentals of the currently selected customer
std::vector<RentalMenuItem> customerRentals;

// Global variable used in the detail printing callback (per requested style)
int currentCustomerID = -1;

// Forward Declarations
void viewCustomer();
void runRentalQuery(int choice);
void displayCustomerDetails(int customerId);
void displayRentalDetails(int rentalId);
int displayCustomerMenuAndGetId(sqlite3 *db);
int displayRentalMenuAndGetId(sqlite3 *db, int customerId);

// --- CALLBACK FUNCTIONS for sqlite3_exec ---

// 1. Callback for populating the global allCustomers vector
int customerListCallback(void *data, int argc, char** argv, char** columnNames) {
    // Expects: customer_id, first_name, last_name
    
    // Check for enough columns and non-null values for the essentials
    if (argc >= 3 && argv[0] != NULL && argv[1] != NULL && argv[2] != NULL) {
        CustomerMenuItem customer;
        customer.id = std::atoi(argv[0]);
        customer.firstName = argv[1];
        customer.lastName = argv[2];
        allCustomers.push_back(customer);
    }
    return 0; // Return 0 to continue processing rows
}

// 2. Callback for populating the global customerRentals vector
int rentalListCallback(void *data, int argc, char** argv, char** columnNames) {
    // Expects: rental_id, rental_date
    
    // Check for enough columns and non-null values for the essentials
    if (argc >= 2 && argv[0] != NULL && argv[1] != NULL) {
        RentalMenuItem rental;
        rental.id = std::atoi(argv[0]);
        rental.rentalDate = argv[1];
        customerRentals.push_back(rental);
    }
    return 0;
}


// --- QUERY WRAPPER FUNCTIONS using sqlite3_exec ---

// Executes the query to load all customers into the global vector
void loadAllCustomers(sqlite3 *db) {
    // Only load if the vector is empty
    if (allCustomers.empty()) {
        const char *sql = "SELECT customer_id, first_name, last_name FROM customer ORDER BY customer_id;";
        char *errorMsg = nullptr;
        
        // Execute the query, using the callback to populate the vector
        int rc = sqlite3_exec(db, sql, customerListCallback, nullptr, &errorMsg);
        
        if (rc != SQLITE_OK) {
            std::cerr << "SQL Customer load error: " << errorMsg << std::endl;
            sqlite3_free(errorMsg);
        }
    }
}

// Executes the query to load all rentals for a given customer into the global vector
void loadCustomerRentals(sqlite3 *db, int customerId) {
    // NOTE: sqlite3_exec does not support dynamic binding like prepared statements.
    // We must build the query string dynamically for this case.
    
    // Clear previous rentals
    customerRentals.clear(); 
    
    std::string sql = "SELECT rental_id, rental_date FROM rental WHERE customer_id = " 
                      + std::to_string(customerId) + " ORDER BY rental_id;";
                      
    char *errorMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), rentalListCallback, nullptr, &errorMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "SQL Rental load error: " << errorMsg << std::endl;
        sqlite3_free(errorMsg);
    }
}


// --- DETAIL VIEW FUNCTIONS (Still use prepared statements for robustness on single row lookups) ---

// Executes a prepared statement to retrieve and print details for a single customer.
void displayCustomerDetails(int customerId) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    
    // Open database
    rc = sqlite3_open("sakila.db", &db);
    if (rc != SQLITE_OK) {
        std::cout << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Query to get customer information
    std::string sql = "SELECT c.customer_id, c.first_name, c.last_name, a.address, y.city, a.phone, c.email, c.last_update "
                      "FROM customer c "
                      "JOIN address a ON c.address_id = a.address_id "
                      "JOIN city y ON a.city_id = y.city_id "
                      "WHERE c.customer_id = ?";

    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cout << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // Bind the customer ID
    sqlite3_bind_int(stmt, 1, customerId);

    // Execute query
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Get the data from the query
        std::string firstName = (const char*)sqlite3_column_text(stmt, 1);
        std::string lastName = (const char*)sqlite3_column_text(stmt, 2);
        std::string address = (const char*)sqlite3_column_text(stmt, 3);
        std::string city = (const char*)sqlite3_column_text(stmt, 4);
        std::string phone = (const char*)sqlite3_column_text(stmt, 5);
        std::string email = (const char*)sqlite3_column_text(stmt, 6);
        std::string lastUpdate = (const char*)sqlite3_column_text(stmt, 7);
        
        // Display customer information
        std::cout << "----Customer Information----" << std::endl;
        std::cout << "Name: " << firstName << " " << lastName << std::endl;
        std::cout << "Address: " << address << std::endl;
        std::cout << "City: " << city << std::endl;
        std::cout << "Phone Number: " << phone << std::endl;
        std::cout << "Email: " << email << std::endl;
        std::cout << "Last Update: " << lastUpdate << std::endl;
    } else {
        std::cout << "Customer not found" << std::endl;
    }

    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Executes a prepared statement to retrieve and print details for a single rental.
void displayRentalDetails(int rentalId) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open("sakila.db", &db);

    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database for rental details: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char *sql =
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

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Rental detail prep error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(stmt, 1, rentalId);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string rentalDate = (const char*)sqlite3_column_text(stmt, 0) ? (const char*)sqlite3_column_text(stmt, 0) : "";
        std::string staffFn    = (const char*)sqlite3_column_text(stmt, 1) ? (const char*)sqlite3_column_text(stmt, 1) : "";
        std::string staffLn    = (const char*)sqlite3_column_text(stmt, 2) ? (const char*)sqlite3_column_text(stmt, 2) : "";
        std::string custFn     = (const char*)sqlite3_column_text(stmt, 3) ? (const char*)sqlite3_column_text(stmt, 3) : "";
        std::string custLn     = (const char*)sqlite3_column_text(stmt, 4) ? (const char*)sqlite3_column_text(stmt, 4) : "";
        std::string filmTitle  = (const char*)sqlite3_column_text(stmt, 5) ? (const char*)sqlite3_column_text(stmt, 5) : "";
        std::string filmDesc   = (const char*)sqlite3_column_text(stmt, 6) ? (const char*)sqlite3_column_text(stmt, 6) : "";
        double rentalRate      = sqlite3_column_double(stmt, 7);
        const char* returnDateCstr = (const char*)sqlite3_column_text(stmt, 8);
        std::string returnDate = returnDateCstr ? returnDateCstr : "N/A";

        std::cout << "Rental Date: " << rentalDate << std::endl;
        std::cout << "Staff: " << staffFn << " " << staffLn << std::endl;
        std::cout << "Customer: " << custFn << " " << custLn << std::endl;
        std::cout << "Film Information:" << std::endl;
        
        std::cout << std::fixed << std::setprecision(2);
        std::cout << filmTitle << " - " << filmDesc << " $" << rentalRate << std::endl;
        
        std::cout << "Return Date: " << returnDate << std::endl;
        
    } else if (rc != SQLITE_DONE) {
         std::cerr << "SQL execution error: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


// --- MENU FUNCTIONS (Handles PAGINATION and USER INPUT on the globally loaded vectors) ---

// Clears the input buffer state after a failure (matching the old style removal)
void clearInputBuffer() {
    if (std::cin.fail()) {
        std::cin.clear(); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// Displays the paginated customer menu and handles input validation.
int displayCustomerMenuAndGetId(sqlite3 *db) {
    
    // Load customer data using the exec/callback method if not already loaded
    loadAllCustomers(db);
    
    int totalRows = allCustomers.size();
    if (totalRows == 0) {
        std::cout << "No customers found." << std::endl;
        return -1;
    }

    // --- Dynamic Rows Per Page Input ---
    int rowsPerPage;
    while (true) {
        std::cout << "There are " << totalRows << " rows in the result.  How many do you want to see per page?" << std::endl;
        std::cout << "Enter Choice: ";
        
        if (!(std::cin >> rowsPerPage)) {
            std::cerr << "Invalid input. Please enter a number." << std::endl;
            clearInputBuffer();
            continue;
        }
        
        if (rowsPerPage <= 0) {
            std::cout << "Rows per page must be a positive number. Defaulting to 10." << std::endl;
            rowsPerPage = 10;
        }
        std::cout << rowsPerPage << std::endl; 
        break;
    }
    // --- End Dynamic Rows Per Page Input ---

    int page = 0;
    
    while (true) {
        int start = page * rowsPerPage; 
        int end = std::min(start + rowsPerPage, totalRows); 
        int currentOptionNum = start + 1;
        
        std::cout << "Please choose the customer you want to see rentals for (enter 0 to go to the next page";
        if (page > 0) {
             std::cout << " or -1 to go to the previous page";
        }
        std::cout << "):" << std::endl;

        for (int i = start; i < end; ++i) {
            std::cout << currentOptionNum++ << ". " 
                      << allCustomers[i].id << " - " 
                      << allCustomers[i].lastName << " " 
                      << allCustomers[i].firstName << std::endl;
        }

        int menuChoice;
        std::cout << "Enter Choice: ";
        
        if (!(std::cin >> menuChoice)) {
            std::cerr << "Invalid input. Please enter a number." << std::endl;
            clearInputBuffer();
            continue;
        }
        
        if (menuChoice == 0) {
            if (end < totalRows) { 
                page++; 
            } else { 
                std::cout << "You are on the last page." << std::endl; 
            }
            continue;
        } else if (menuChoice == -1) {
            if (page > 0) { 
                page--; 
            } else { 
                std::cout << "You are on the first page." << std::endl; 
            }
            continue;
        }
        
        if (menuChoice >= start + 1 && menuChoice <= end) {
            int index = menuChoice - 1; 
            return allCustomers[index].id;
        } else {
            std::cerr << "Error: Please choose a valid customer number between " 
                      << start + 1 << " and " << end << "." << std::endl;
        }
    }
}


// Displays the paginated rental menu for a specific customer and handles input validation.
int displayRentalMenuAndGetId(sqlite3 *db, int customerId) {
    
    // Load rentals using the exec/callback method
    loadCustomerRentals(db, customerId);
    
    int totalRows = customerRentals.size();
    if (totalRows == 0) {
        std::cout << "No rentals found for this customer." << std::endl;
        return -1;
    }

    // --- Dynamic Rows Per Page Input ---
    int rowsPerPage;
    while (true) {
        std::cout << "There are " << totalRows << " rows in the result.  How many do you want to see per page?" << std::endl;
        std::cout << "Enter Choice: ";
        
        if (!(std::cin >> rowsPerPage)) {
            std::cerr << "Invalid input. Please enter a number." << std::endl;
            clearInputBuffer();
            continue;
        }
        
        if (rowsPerPage <= 0) {
            std::cout << "Rows per page must be a positive number. Defaulting to 10." << std::endl;
            rowsPerPage = 10;
        }
        std::cout << rowsPerPage << std::endl; 
        break;
    }
    // --- End Dynamic Rows Per Page Input ---
    
    int page = 0;
    
    while (true) {
        int start = page * rowsPerPage; 
        int end = std::min(start + rowsPerPage, totalRows); 
        int currentOptionNum = start + 1;
        
        std::cout << "Please choose the rental you want to see (enter 0 to go to the next page";
        if (page > 0) {
             std::cout << " or -1 to go to the previous page";
        }
        std::cout << "):" << std::endl;

        for (int i = start; i < end; ++i) {
            std::cout << currentOptionNum++ << ". " 
                      << customerRentals[i].id << " - " 
                      << customerRentals[i].rentalDate << std::endl;
        }

        int menuChoice;
        std::cout << "Enter Choice: ";
        
        if (!(std::cin >> menuChoice)) {
            std::cerr << "Invalid input. Please enter a number." << std::endl;
            clearInputBuffer();
            continue;
        }
        
        if (menuChoice == 0) {
            if (end < totalRows) { page++; } else { std::cout << "You are on the last page." << std::endl; }
            continue;
        } else if (menuChoice == -1) {
            if (page > 0) { page--; } else { std::cout << "You are on the first page." << std::endl; }
            continue;
        }
        
        if (menuChoice >= start + 1 && menuChoice <= end) {
            int index = menuChoice - 1; 
            return customerRentals[index].id;
        } else {
            std::cerr << "Error: Please choose a valid rental number between " 
                      << start + 1 << " and " << end << "." << std::endl;
        }
    }
}

// Implements assignment Option 2: View Customer Information
void viewCustomer() {
    sqlite3 *db;
    int rc = sqlite3_open("sakila.db", &db);

    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    
    std::cout << "Please choose the customer you want to see:" << std::endl; 
    int selectedId = displayCustomerMenuAndGetId(db);

    sqlite3_close(db);

    if (selectedId > 0) {
        displayCustomerDetails(selectedId);
    }
}


// Implements assignment Option 1: View the rentals for a customer
void runRentalQuery(int choice) {
    sqlite3 *db;
    int rc = sqlite3_open("sakila.db", &db); 

    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    
    // 1. Get Customer ID (Handles customer menu pagination)
    int selectedCustomerId = displayCustomerMenuAndGetId(db);

    if (selectedCustomerId > 0) {
        // 2. Get Rental ID for that customer (Handles rental menu pagination)
        // Note: The DB connection is still open for the rental menu logic
        int selectedRentalId = displayRentalMenuAndGetId(db, selectedCustomerId);

        if (selectedRentalId > 0) {
            // Close the current DB connection before opening a new one for details
            sqlite3_close(db); 
            // 3. Display Rental Details
            displayRentalDetails(selectedRentalId); 
            return;
        }
    }
    sqlite3_close(db); 
}

// --- MAIN PROGRAM EXECUTION (Refactored to match goto style) ---

int main () {
    int choice;
    
    // Use the goto label as requested
    enterChoice: 

    std::cout << "Welcome to Sakila" << std::endl;
    std::cout << "Please choose an option (enter -1 to quit) :" << std::endl;
    std::cout << "1. View the rentals for a customer" << std::endl;
    std::cout << "2. View Customer Information" << std::endl;
    std::cout << "Enter Choice: ";
    
    // Simplified input handling as requested by the refactoring style
    std::cin >> choice;

    if (choice == -1) {
        std::exit(0);
    }
    
    if (choice == 1) {
        runRentalQuery(choice); 
    } else if (choice == 2) {
        viewCustomer(); 
    } else {
        std::cout << "Please enter a valid number (1, 2, or -1)." << std::endl;
    }
    
    std::cout << std::endl; // Spacer
    goto enterChoice; // Loop back to the menu
    
    // Unreachable, but required return type
    return 0;
}