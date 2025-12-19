/* Program name: main.cpp
    Author: Dominic Angelo
    Date last updated: 12/14/25
    Purpose: Interact with hebrews.db
*/

#include <iostream>
#include <vector>
#include <sqlite3.h>

using namespace std;

struct customer {
    int id;
    string firstName;
    string lastName;
    string address;
    string city;
    string state;
    string zip;
    string phone;
    string email;
    string password;
    bool hasSubscription = false;
};

struct subscription {
    int id;
    int customerId;
    int frequency;
    string startDate;
    string lastEdit;
    bool status;
};

struct roast {
    int id;
    int batchId;
    string coffeeName;
    string species;
    string growthRegion;
    string farm;
    string roastDepth;
    string roastDate;
};
 
sqlite3 *db;

vector<customer> customers;
vector<roast> roasts;

//load data
void loadCustomers();
void loadRoasts();
void loadSubscriptions();
bool initializeDatabase();
//callbacks
int customerCallback(void *data, int argc, char** argb, char** columnNames);
int subscriptionCallback(void *data, int argc, char** argb, char** columnNames);
int roastCallback(void *data, int argc, char** argb, char** columnNames);
//create
int createCustomer();
void createSubscription(int customerIndex);
void insertSubscriptionItem(int subscriptionId, int frequency);
//menu options
void addRoast();
void addProduct();
//update
void updateSubscription();
void updateCustomer();
//delete
void deleteSubscription();
//view reports
void viewCustomerSubscriptionDetails();
void viewSubscriptionItemsReport();
//login
int customerLogin();
//display roasts
void displayRoasts();

int main() {
    initializeDatabase();
    int menuChoice;
    while (true) {
        cout << "Welcome to Hebrew's Coffee!" << endl << endl;
        cout << "Please choose an option, enter -1 to quit." << endl;
        cout << "==========================================" << endl;
        cout << endl << "---Transaction---" << endl;
        cout << "Enter 1 to become a new customer and create a new subscription" << endl;
        cout << endl << "---Add Options---" << endl;
        cout << "Enter 2 to add a new roast to catalog (admin)" << endl;
        cout << "Enter 3 to add a new product (admin)" << endl;
        cout << endl << "---Update Option--" << endl;
        cout << "Enter 4 to update subscription" << endl;
        cout << "Enter 5 to update customer" << endl;
        cout << endl << "--- Delete Option---" << endl;
        cout << "Enter 6 to delete a subscription" << endl;
        cout << endl << "--- Report Option---" << endl;
        cout << "Enter 7 to view customer subscription details" << endl;
        cout << "Enter 8 to view subscription items report" << endl;

        cin >> menuChoice;

        cin.ignore(10000, '\n');

        if (cin.fail()) {
            cout << "You entered something invalid" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (menuChoice == -1) {
            break;
        }
  
        switch (menuChoice) {
            case 1: {
                int newCustomerIndex = createCustomer();
                if (newCustomerIndex != -1) {
                    createSubscription(newCustomerIndex);
                }
                break;
            }
            case 2:
                addRoast();
                break;
            case 3:
                addProduct();
                break;
            case 4:
                updateSubscription();
                break;
            case 5:
                updateCustomer();
                break;
            case 6:
                deleteSubscription();
                break;
            case 7:
                viewCustomerSubscriptionDetails();
                break;
            case 8:
                viewSubscriptionItemsReport();
                break;
            default:
                break;
        }
    } 
    sqlite3_close(db);
    return 0;
}

bool initializeDatabase() {
    int database = sqlite3_open("hebrews.db", &db);
    if (database != SQLITE_OK) {
        cerr << "Error with initializing database" << endl;
        return false;
    } else {
        cout << "Database initilization complete and successful!" << endl;
        loadCustomers();
        loadSubscriptions();
        loadRoasts();
        return true;
    }
}

int createCustomer() {
    customer newCustomer;
    sqlite3_stmt *statement;
    const char* createCustomerQuery = 
    "INSERT INTO customer ("
    "   first_name, "
    "   last_name, "
    "   address, "
    "   city, "
    "   state, "
    "   zip, "
    "   phone, "
    "   email, "
    "   password"
    ") "
    "SELECT "
    "? AS first_name, "
    "? AS last_name, "
    "? AS address, "
    "? AS city, "
    "? AS state, "
    "? AS zip, "
    "? AS phone, "
    "? AS email, "
    "? AS password;";

    cout << "Enter your first name: ";
    getline(cin, newCustomer.firstName);
    cout << "Enter your last name: ";
    getline(cin, newCustomer.lastName);
    cout << "Enter your street address (no city, state, or zip): ";
    getline(cin, newCustomer.address);
    cout << "Enter your city: ";
    getline(cin, newCustomer.city);
    cout << "Enter your state: ";
    getline(cin, newCustomer.state);
    cout << "Enter your zip: ";
    getline(cin, newCustomer.zip);
    cout << "Enter your phone: ";
    getline(cin, newCustomer.phone);
    cout << "Enter your email: ";
    getline(cin, newCustomer.email);
    cout << "Enter your password: ";
    getline(cin, newCustomer.password);

    int rc = sqlite3_prepare_v2(db, createCustomerQuery, -1, &statement, NULL);

    if (rc != SQLITE_OK) {
        cerr << "Erro with preparing insert query" << endl;
        return -1;
    }

    sqlite3_bind_text(statement, 1, newCustomer.firstName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 2, newCustomer.lastName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 3, newCustomer.address.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 4, newCustomer.city.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 5, newCustomer.state.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 6, newCustomer.zip.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 7, newCustomer.phone.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 8, newCustomer.email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 9, newCustomer.password.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(statement);

    if (rc != SQLITE_DONE) {
        cerr << "Create customer query failed" << endl;
        return -1;
    } else {
        cout << "Successfully entered new customer" << endl;
        cout << "Enter your subscription details" << endl;
    }

    newCustomer.id = sqlite3_last_insert_rowid(db);

    customers.push_back(newCustomer);
    sqlite3_finalize(statement);
    return customers.size() - 1;
}

void loadCustomers() {
    const char *getCustomersCommand = 
        "SELECT "
        "customer_id, "
        "first_name, "
        "last_name, "
        "address, "
        "city, "
        "state, "
        "zip, "
        "phone, "
        "email, "
        "password "
        "FROM "
        "customer "
        "ORDER BY "
        "customer_id";

        char *errorMsg = nullptr;
        int rc = sqlite3_exec(db, getCustomersCommand, customerCallback, nullptr, &errorMsg);
        sqlite3_free(errorMsg); //prevents memory leaks
}

void loadSubscriptions() {
    const char *getSubscriptionsCommand = 
        "SELECT "
        "subscription_id, "
        "customer_id, "
        "frequency, "
        "start_date, "
        "last_edit, "
        "status "
        "FROM "
        "subscription "
        "ORDER BY "
        "subscription_id;";

        char *errorMsg = nullptr;
        int rc = sqlite3_exec(db, getSubscriptionsCommand, subscriptionCallback, nullptr, &errorMsg);
        sqlite3_free(errorMsg);
}

void loadRoasts() {
    const char *getRoastsCommand = 
    "SELECT "
    "roast_id, "
    "batch_id, "
    "coffee_name, "
    "species, "
    "growth_region, "
    "farm, "
    "roast_depth, "
    "roast_date "
    "FROM "
    "roast "
    "ORDER BY "
    "roast_id;";
    
    char *errorMsg = nullptr;
    int rc = sqlite3_exec(db, getRoastsCommand,  roastCallback, nullptr, &errorMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error loading roasts: " << errorMsg << endl;
    }
    sqlite3_free(errorMsg);
}

int customerCallback(void *data, int argc, char** argv, char** columnNames) {
    customer newCustomer;
    
    if (argv[0] != NULL && argv[1] != NULL && argv[2] != NULL && argv[3] != NULL && argv[4] != NULL &&
        argv[5] != NULL && argv[6] != NULL && argv[7] != NULL && argv[8] != NULL && argv[9] != NULL) {
            newCustomer.id = atoi(argv[0]);
            newCustomer.firstName = argv[1];
            newCustomer.lastName = argv[2];
            newCustomer.address = argv[3];
            newCustomer.city = argv[4];
            newCustomer.state = argv[5];
            newCustomer.zip = argv[6];
            newCustomer.phone = argv[7];
            newCustomer.email = argv[8];
            newCustomer.password = argv[9];
            customers.push_back(newCustomer);
        }
        return 0;
}

int subscriptionCallback(void *data, int argc, char** argv, char** columnNames) {
    subscription newSubscription;
    if (argv[0] != NULL && argv[1] != NULL && argv[2] != NULL && argv[3] != NULL && argv[4] != NULL && argv[5] != NULL) {
        int customerId = atoi(argv[1]);
        bool status = atoi(argv[5]);

        if (status) {
            for (int i = 0; i < customers.size(); i++) {
                if (customers[i].id == customerId) {
                    customers[i].hasSubscription = true;
                    break;
                }
            }
        }
    }
    return 0;
}

int roastCallback(void *data, int argc, char** argv, char** columnNames) {
    roast newRoast;

    if (argv[0] != NULL && argv[1] != NULL && argv[2] != NULL && argv[3] != NULL && argv[4] != NULL &&
        argv[5] != NULL && argv[6] != NULL && argv[7] != NULL) {
            newRoast.id = atoi(argv[0]);
            newRoast.batchId = atoi(argv[1]);
            newRoast.coffeeName = argv[2];
            newRoast.species = argv[3];
            newRoast.growthRegion = argv[4];
            newRoast.farm = argv[5];
            newRoast.roastDepth = argv[6];
            newRoast.roastDate = argv[7];
            roasts.push_back(newRoast);
        }
        return 0;
}

int customerLogin() {
    customer currentCustomer;
   
    while (true) {
        cout << "--- Customer Login ---" << endl;
        cout << "Enter -1 at any time to quit" << endl;
        cout << "Enter your email: ";
        getline(cin, currentCustomer.email);
    
        if (currentCustomer.email == "-1") {
            return -1;
        }

        int customerIndex = -1;

        for (int i = 0; i < customers.size(); i++) {
            if (currentCustomer.email == customers[i].email) {
                customerIndex = i;
                break;
            }
        }

        if (customerIndex == -1) {
            cout << "Email not found. Please try again" << endl;
            continue;
        }

        while (true) {
            cout << "Enter your password: ";
            getline(cin, currentCustomer.password);

            if (currentCustomer.password == "-1") {
                return -1;
            }

            if (currentCustomer.password == customers[customerIndex].password) {
                return customerIndex;
            } else {
                cout << "Password not correct. Please try again." << endl;
            }
        }

    }
}

void displayRoasts() {
    cout << "Roasts:" << endl;
    for (int i = 0; i < roasts.size(); i++) {
        cout << "   ID: " << roasts[i].id << " - " << roasts[i].coffeeName << endl;
        cout << "   Roast: " << roasts[i].roastDepth << " | Origin: " << roasts[i].growthRegion << endl;
        cout << "   Farm: " << roasts[i].farm << endl;
        cout << endl;
    }
}

void createSubscription(int customerIndex = -1) {
    const char* insertSubscriptionQuery = 
    "INSERT INTO subscription ("
    "   customer_id, "
    "   frequency, "
    "   start_date, "
    "   last_edit, "
    "   status"
    ") "
    "VALUES ("
    "?, "
    "?, "
    "CURRENT_DATE, "
    "CURRENT_DATE, "
    "1);";

    if (customerIndex == -1) {
        customerIndex = customerLogin();
        if (customerIndex == -1) {
            return;
        }
    }

    int customerId = customers[customerIndex].id;

    if (customers[customerIndex].hasSubscription) {
        int choice;
        cout << "You already have a subscription" << endl;
        cout << "Would you like to edit your subscription?" << endl;
        cout << "Enter 1 for yes and -1 to quit" << endl;
        cin >> choice;
        cin.ignore();

        if (choice == -1) {
            return;
        } else {
            updateSubscription();
        }

    } else {
        int choice;
        cout << "You do not have a subscription would you like to create one?" << endl;
        cout << "Enter 1 for yes and -1 to quit" << endl;
        cin >> choice;

        if (choice == -1) {
            return;
        } else if (choice == 1) {
            int frequency;
            
            while(true) {
                cout << "Enter how many days you want between shipments must be 7, 14, or 30: ";
                cin >> frequency;
                if (frequency != 7 && frequency != 14 && frequency !=30 ) {
                    cout << "Number must be 7, 14, or 30. Please try again" << endl;
                    continue;
                } else {
                    break;
                }
            }
            cin.ignore();

            sqlite3_stmt *statement;
            int rc = sqlite3_prepare_v2(db, insertSubscriptionQuery, -1, &statement, NULL);

            if (rc != SQLITE_OK) {
                cerr << "Error preparing insert subscription query" << endl;
                return;
            }

            sqlite3_bind_int(statement, 1, customerId);
            sqlite3_bind_int(statement, 2, frequency);

            rc = sqlite3_step(statement);

            if (rc != SQLITE_DONE) {
                cerr << "Insert subscription query failed" << endl;
            } else {
                cout << "Successfully created new subscription!" << endl;
                customers[customerIndex].hasSubscription = true;
            }

            int subscriptionId = sqlite3_last_insert_rowid(db);
            sqlite3_finalize(statement);
            insertSubscriptionItem(subscriptionId, frequency);
        }
    }
}

void insertSubscriptionItem(int subscriptionId, int frequency) {
    const char* insertSubscriptionItemQuery =  
    "INSERT INTO subscription_item ("
    "   subscription_id, "
    "   roast_id, "
    "   selection, "
    "   quantity"
    ") "
    "VALUES (?, ?, ?, ?);";

    int maxItems;
    if (frequency == 7) {
        maxItems = 2;
    } else if (frequency == 14) {
        maxItems = 4;
    } else if (frequency == 30) {
        maxItems = 8;
    } else {
        cout << "Invalid frequency" << endl;
        return;
    }

    cout << "You can add up to " << maxItems << " roasts to your subscription" << endl;
    int itemcount = 0;
    int roastChoice;
    int selectionChoice;
    string selection;
    int quantity;



    while (itemcount < maxItems) {
        displayRoasts();
        cout << "Enter roast ID to add (or -1 to finish) : ";
        cin >> roastChoice;
        if (cin.fail()) {
            cout << "invalid input. please enter a correct number" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (roastChoice == -1) {
            break;
        }
        bool validRoast = false;
        for (int i = 0; i < roasts.size(); i++) {
            if (roasts[i].id == roastChoice) {
                validRoast = true;
                break;
            }
        }

        if (!validRoast) {
            cout << "invalid roast id. please try again" << endl;
            continue;
        }

        cout << "\nSelect grind type:" << endl;
        cout << "1. Whole Bean" << endl;
        cout << "2. Drip Grind" << endl;
        cout << "3. Espresso Grind" << endl;
        cout << "Enter choice: ";
        cin >> selectionChoice;
        
        switch(selectionChoice) {
            case 1:
                selection = "Whole Bean";
                break;
            case 2:
                selection = "Drip Grind";
                break;
            case 3:
                selection = "Espresso Grind";
                break;
            default:
                cout << "Invalid selection. Defaulting to Whole Bean." << endl;
                selection = "Whole Bean";
        }

        cout << "Enter quantity (number of bags): ";
        cin >> quantity;
        if ((itemcount + quantity) > maxItems) {
            cout << "You cannot take that many please try again" << endl;
            continue;
        } 
        itemcount += quantity;
        cin.ignore(1000,'\n');
        sqlite3_stmt *statement;
        int rc = sqlite3_prepare_v2(db, insertSubscriptionItemQuery, -1, &statement, NULL);

        if (rc != SQLITE_OK) {
            cerr << "Error preparing subscription insert" << endl;
        }
        
        sqlite3_bind_int(statement, 1, subscriptionId);
        sqlite3_bind_int(statement, 2, roastChoice);
        sqlite3_bind_text(statement, 3, selection.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(statement, 4, quantity);
        
        rc = sqlite3_step(statement);

        if (rc != SQLITE_DONE) {
            cerr << "Failed to add to subscription item" << endl;
        }

        sqlite3_finalize(statement);
    }
}

void addRoast() {
    roast newRoast;
    sqlite3_stmt *statement;

    const char* insertRoastQuery = 
    "INSERT INTO roast ("
    "   batch_id, "
    "   coffee_name, "
    "   species, "
    "   growth_region, "
    "   farm, "
    "   roast_depth, "
    "   roast_date"
    ") "
    "VALUES (?, ?, ?, ?, ?, ?, CURRENT_DATE);";

    cout << endl << "--- Add New Roast ---" << endl;
    cout << "Enter batch ID: ";
    cin >> newRoast.batchId;
    cin.ignore();

    cout << "Enter coffee name: ";
    getline(cin, newRoast.coffeeName);

    cout << "enter species: ";
    getline(cin, newRoast.species);

    cout << "Enter growth region: ";
    getline(cin, newRoast.growthRegion);

    cout << "Enter farm name: ";
    getline(cin, newRoast.farm);

    cout << "Enter roast depth (light/medium/dark): ";
    getline(cin, newRoast.roastDepth);

    int rc = sqlite3_prepare_v2(db, insertRoastQuery, -1, &statement, NULL);

    if (rc != SQLITE_OK) {
        cerr << "Error preparing insert roast query" << endl;
        return;
    }

    sqlite3_bind_int(statement, 1, newRoast.batchId);
    sqlite3_bind_text(statement, 2, newRoast.coffeeName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 3, newRoast.species.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 4, newRoast.growthRegion.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 5, newRoast.farm.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 6, newRoast.roastDepth.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(statement);

    if (rc != SQLITE_DONE) {
        cerr << "Failed to add roast to database" << endl;
    } else {
        cout << "Successfully added new roast" << endl;
        newRoast.id = sqlite3_last_insert_rowid(db);
        roasts.push_back(newRoast);
    }
    sqlite3_finalize(statement);
}

void addProduct() {}

void updateSubscription() {
    
    const char* updateSubscriptionQuery = 
    "UPDATE subscription "
    "SET frequency = ?, "
    "last_edit = CURRENT_DATE "
    "WHERE customer_id = ?;";

    int frequency;

    int customerIndex = customerLogin();

    if (customerIndex == -1) {
        return;
    }
    
    int customerId = customers[customerIndex].id;

    if (customers[customerIndex].hasSubscription) { 
        int choice;
        cout << "You already have a subscription" << endl;
        cout << "Would you like to edit your subscription" << endl;
        while (true) {
            cout << "Enter 1 for yes and -1 to quit: " << endl;
            cin >> choice;
            if (cin.fail() || (choice != 1 && choice != -1)) {
                cout << "Please enter a valid number" << endl;
                continue;
            } 
            if (choice == -1) {
                return;
            } else {
                break;
            }
        }
        
        while(true) {
            cout << "Enter how many days you want between shipments must be 7, 14, or 30: ";
            cin >> frequency;
            if (frequency != 7 && frequency != 14 && frequency !=30 ) {
                cout << "Number must be 7, 14, or 30. Please try again" << endl;
                continue;
            } else {                    
                break;
            }
        }
        cin.ignore();
        

        sqlite3_stmt *statement;

        int rc = sqlite3_prepare_v2(db, updateSubscriptionQuery, -1, &statement, NULL);

        if (rc != SQLITE_OK) {
            cout << "Error with prepareing update subscription query" << endl;
            return;
        }

        sqlite3_bind_int(statement, 1, frequency);
        sqlite3_bind_int(statement, 2, customerId);

        rc = sqlite3_step(statement);

        if (rc != SQLITE_DONE) {
            cerr << "update subscription query failed" << endl;
        }

        sqlite3_finalize(statement);

    }
}

void deleteSubscription() {
    const char* deleteSubscriptionQuery = 
    "UPDATE subscription "
    "SET status = 0, "
    "   last_edit = CURRENT_DATE "
    "WHERE customer_id = ?;";

    int customerIndex = customerLogin();

    if (customerIndex == -1) {
        return;
    }

    if (!customers[customerIndex].hasSubscription) {
        cout << "you do not have an active subscription to delete." << endl;
        return;
    }

    cout << endl << "--- Cancel Subscription ---" << endl;
    cout << "Are you sure you want to cancel your subscription?" << endl;
    cout << "Enter 1 to confirm, -1 to cancel" << endl;

    int confirm;
    cin >> confirm;
    cin.ignore();

    if (confirm != 1) {
        cout << "Subscription cancellation cancelled" << endl;
        return;
    }

    sqlite3_stmt *statement;
    int rc = sqlite3_prepare_v2(db, deleteSubscriptionQuery, -1, &statement, NULL);

    if (rc != SQLITE_OK){
        cerr << "Error preparing delete subscription query" << endl;
        return;
    }

    sqlite3_bind_int(statement, 1, customers[customerIndex].id);

    rc = sqlite3_step(statement);

    if (rc != SQLITE_DONE) {
        cout <<"Delete subscription query failed" << endl;
    } else {
        cout << "Successfully cancelled subscription" << endl;
        customers[customerIndex].hasSubscription = false;
    }

    sqlite3_finalize(statement);
}

void viewCustomerSubscriptionDetails() {
    const char* joinQuery = 
    "SELECT "
    "   c.customer_id, "
    "   c.first_name, "
    "   c.last_name, "
    "   c.email, "
    "   c.city, "
    "   c.state, "
    "   s.subscription_id, "
    "   s.frequency, "
    "   s.start_date, "
    "   s.last_edit, "
    "   s.status "
    "FROM customer c "
    "INNER JOIN subscription s ON c.customer_id = s.customer_id "
    "WHERE s.status = 1 "
    "ORDER BY c.last_name, c.first_name;";

    sqlite3_stmt *statement;
    int rc = sqlite3_prepare_v2(db, joinQuery, -1, &statement, NULL);

    if (rc != SQLITE_OK) {
        cerr << "Error preparing join query" << endl;
        return;
    }

    cout << endl << "--- Customer Subscription Details ---" << endl;
    int count = 0;
    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        count++;
        cout << "Customer #" << count << endl;
        cout << "Customer ID: " << sqlite3_column_int(statement, 0) << endl;
        cout << "Name: " << sqlite3_column_text(statement, 1) << " " 
             << sqlite3_column_text(statement, 2) << endl;
        cout << "Email: " << sqlite3_column_text(statement, 3) << endl;
        cout << "Location: " << sqlite3_column_text(statement, 4) << ", " 
             << sqlite3_column_text(statement, 5) << endl;
        cout << "\nSubscription Details:" << endl;
        cout << "  Subscription ID: " << sqlite3_column_int(statement, 6) << endl;
        cout << "  Frequency: Every " << sqlite3_column_int(statement, 7) << " days" << endl;
        cout << "  Start Date: " << sqlite3_column_text(statement, 8) << endl;
        cout << "  Last Edited: " << sqlite3_column_text(statement, 9) << endl;
        cout << "  Status: Active" << endl;
    }

    if (count == 0) {
        cout << "No subscriptions found" << endl;
    }

    sqlite3_finalize(statement);
}

void viewSubscriptionItemsReport() {
    const char* joinQuery = 
    "SELECT "
    "   s.subscription_id, "
    "   c.first_name, "
    "   c.last_name, "
    "   s.frequency, "
    "   r.coffee_name, "
    "   r.roast_depth, "
    "   r.growth_region, "
    "   si.selection, "
    "   si.quantity "
    "FROM subscription s "
    "INNER JOIN customer c ON s.customer_id = c.customer_id "
    "INNER JOIN subscription_item si ON s.subscription_id = si.subscription_id "
    "INNER JOIN roast r ON si.roast_id = r.roast_id "
    "WHERE s.status = 1 "
    "ORDER BY s.subscription_id, r.coffee_name;";

    sqlite3_stmt *statement;
    int rc = sqlite3_prepare_v2(db, joinQuery, -1, &statement, NULL);

    if (rc != SQLITE_OK) {
        cerr << "error preparing join query for subcription items" << endl;
        return;
    }

    cout << endl << "--- Subscription Items Report ---" << endl;
    int currentSubId = -1;
    int count = 0;

    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        int subId = sqlite3_column_int(statement, 0);

        if (subId != currentSubId) {
            if (currentSubId != -1) {
                cout << "----------" << endl;
            }
            currentSubId = subId;
            cout << "Subscription ID: " << subId << endl;
            cout << "Customer: " << sqlite3_column_text(statement, 1) << " " 
                 << sqlite3_column_text(statement, 2) << endl;
            cout << "Frequency: Every " << sqlite3_column_int(statement, 3) << " days" << endl;
            cout << "\nItems in this subscription:" << endl;            
        }

        cout << "  - " << sqlite3_column_text(statement, 4) 
             << " (" << sqlite3_column_text(statement, 5) << ")" << endl;
        cout << "    Origin: " << sqlite3_column_text(statement, 6) << endl;
        cout << "    Grind: " << sqlite3_column_text(statement, 7) << endl;
        cout << "    Quantity: " << sqlite3_column_int(statement, 8) << " bag(s)" << endl;

        count++;
    }
    if (count == 0) {
        cout << "No subscriptions found" << endl;
    }

    sqlite3_finalize(statement);
}

void updateCustomer() {
    const char* updateCustomerQuery = 
        "UPDATE customer "
        "SET first_name = ?, "
        "    last_name = ?, "
        "    address = ?, "
        "    city = ?, "
        "    state = ?, "
        "    zip = ?, "
        "    phone = ?, "
        "    email = ? "
        "WHERE customer_id = ?;";

    int customerIndex = customerLogin();

    cout <<  endl << "--- Current Customer Info ---" << endl;
    cout << "Current Information:" << endl;
    cout << "Name: " << customers[customerIndex].firstName << " " 
             << customers[customerIndex].lastName << endl;
    cout << "Email: " << customers[customerIndex].email << endl;
    cout << "Phone: " << customers[customerIndex].phone << endl;
    cout << "Address: " << customers[customerIndex].address << endl;
    cout << "City: " << customers[customerIndex].city << endl;
    cout << "State: " << customers[customerIndex].state << endl;
    cout << "Zip: " << customers[customerIndex].zip << endl;
    cout << endl;

    int choice;
    cout << "Would you like to update your information" << endl;
    cout << "Enter 1 for yes and -1 to cancel" << endl;
    cin >> choice; 
    cin.ignore();

    if (choice != -1) {
        cout << "update cancelled" << endl;
    }

    customer updatedCustomer;
    updatedCustomer.id = customers[customerIndex].id;

    cout << "Enter new first name (or press Enter to keep current): ";
    getline(cin, updatedCustomer.firstName);
    if (updatedCustomer.firstName.empty()) {
        updatedCustomer.firstName = customers[customerIndex].firstName;
    }

    cout << "Enter new last name (or press Enter to keep current): ";
    getline(cin, updatedCustomer.lastName);
    if (updatedCustomer.lastName.empty()) {
        updatedCustomer.lastName = customers[customerIndex].lastName;
    }

    cout << "Enter new street address (or press Enter to keep current): ";
    getline(cin, updatedCustomer.address);
    if (updatedCustomer.address.empty()) {
        updatedCustomer.address = customers[customerIndex].address;
    }

    cout << "Enter new city (or press Enter to keep current): ";
    getline(cin, updatedCustomer.city);
    if (updatedCustomer.city.empty()) {
        updatedCustomer.city = customers[customerIndex].city;
    }

    cout << "Enter new state (or press Enter to keep current): ";
    getline(cin, updatedCustomer.state);
    if (updatedCustomer.state.empty()) {
        updatedCustomer.state = customers[customerIndex].state;
    }

    cout << "Enter new zip (or press Enter to keep current): ";
    getline(cin, updatedCustomer.zip);
    if (updatedCustomer.zip.empty()) {
        updatedCustomer.zip = customers[customerIndex].zip;
    }

    cout << "Enter new phone (or press Enter to keep current): ";
    getline(cin, updatedCustomer.phone);
    if (updatedCustomer.phone.empty()) {
        updatedCustomer.phone = customers[customerIndex].phone;
    }

    cout << "Enter new email (or press Enter to keep current): ";
    getline(cin, updatedCustomer.email);
    if (updatedCustomer.email.empty()) {
        updatedCustomer.email = customers[customerIndex].email;
    }

    sqlite3_stmt *statement;
    int rc = sqlite3_prepare_v2(db, updateCustomerQuery, -1, &statement, NULL);

    if (rc != SQLITE_OK) {
        cerr << "Error preparing update customer query" << endl;
        return;
    }

    sqlite3_bind_text(statement, 1, updatedCustomer.firstName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 2, updatedCustomer.lastName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 3, updatedCustomer.address.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 4, updatedCustomer.city.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 5, updatedCustomer.state.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 6, updatedCustomer.zip.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 7, updatedCustomer.phone.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 8, updatedCustomer.email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(statement, 9, updatedCustomer.id);

    rc = sqlite3_step(statement);

    if (rc != SQLITE_DONE) {
        cerr << "update customer query failed" << endl;
    } else {
        cout << "Successfully updated customer information" << endl;
        customers[customerIndex].firstName = updatedCustomer.firstName;
        customers[customerIndex].lastName = updatedCustomer.lastName;
        customers[customerIndex].address = updatedCustomer.address;
        customers[customerIndex].city = updatedCustomer.city;
        customers[customerIndex].state = updatedCustomer.state;
        customers[customerIndex].zip = updatedCustomer.zip;
        customers[customerIndex].phone = updatedCustomer.phone;
        customers[customerIndex].email = updatedCustomer.email;
    }

    sqlite3_finalize(statement);

}