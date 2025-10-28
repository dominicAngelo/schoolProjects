/* Program name: fileIO.cpp
*  Author: Dominic Angelo
*  Date last updated 10/27/25
*  Purpose: Hospital StudyMor Study
*/

#include <fstream>
#include <iostream>


void menu();
void newParticipant();
void collectSurvey();
void displayParticipant(); 
void quit();

int main() {
    std::cout << "Welcome to the Westlake Research Hospital StudyMor Study!\n" << std::endl;
    menu();
}

void menu() {
    int choice;
    std::cout << "1. Add a New Participant\n"
              << "2. Collect Survey for Participant\n"
              << "3. Display Participants\n"
              << "4. Quit\n"
              << std::endl
              << "Please enter a command to continue: ";
    std::cin >> choice;
    std::cout << std::endl;
    std::cin.clear();
    switch (choice) {
        case 1:
            //newParticipant();
            break;
        case 2:
            //collectSurvey();
            break;
        case 3:
            //displayParticipant();
            break;
        case 4:
            //quit();
            break;
    }
}
