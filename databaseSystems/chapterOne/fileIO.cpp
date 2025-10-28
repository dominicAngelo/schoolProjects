/* Program name: fileIO.cpp
*  Author: Dominic Angelo
*  Date last updated 10/27/25
*  Purpose: Hospital StudyMor Study
*/

#include <fstream>
#include <iostream>
#include <vector>

struct participant {
    std::string firstName;
    std::string lastName;
    int age;
    char gender;
    int id;
};

std::vector<participant> participants;

void menu();
void addNewParticipant();
void collectSurvey();
void displayParticipant(); 
void quit();

int main() {
    std::cout << "Welcome to the Westlake Research Hospital StudyMor Study!\n" << std::endl;
    menu();
}

void addNewParticipant() {
    participant newParticipant;
    std::cout << "Enter the participant's first name: ";
    std::getline(std::cin, newParticipant.firstName);
    std::cout << std::endl << "Enter the participant's last name: ";
    std::getline(std::cin, newParticipant.lastName);
    std::cout << std::endl << "Enter " << newParticipant.firstName << " "
              << newParticipant.lastName << "'s age: ";
    std::cin >> newParticipant.age;
    std::cin.ignore(1000, '\n');
    std::cout << std::endl;
    std::cout << std::endl << "Enter " << newParticipant.firstName << " "
              << newParticipant.lastName << "'s gender: ";
    std::cin >> newParticipant.gender;
    std::cin.ignore(1000, '\n');
    std::cout << std::endl;
    
    participants.push_back(newParticipant);
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
            //addNewParticipant();
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
