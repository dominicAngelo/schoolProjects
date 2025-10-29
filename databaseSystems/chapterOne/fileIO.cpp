/* Program name: fileIO.cpp
*  Author: Dominic Angelo
*  Date last updated 10/27/25
*  Purpose: Hospital StudyMor Study
*/

#include <fstream>
#include <iostream>
#include <vector>

std::ofstream participantFile;
std::ofstream surveyFile;

struct participant {
    std::string firstName;
    std::string lastName;
    int age;
    char gender;
    int id;
    bool studyMor;
    std::string takesStudyMor() {
        if (id%2 == 0) {
            studyMor = true;
            return "Yes\n";
        } else {
            studyMor = false;
            return "No\n";
        }
    }
};

std::vector<participant> participants;

void menu();
void addNewParticipant();
void collectSurvey();
void displayParticipant(); 
void quit();

int main() {
    std::cout << "Welcome to the Westlake Research Hospital StudyMor Study!\n" << std::endl;
    while (true) {
        menu();
    }
}

void addNewParticipant() {
    participant newParticipant;
    int newID = 1;
    newID += participants.size();
    newParticipant.id = newID; 
    std::cout << "Enter the participant's first name: ";
    std::cin >> newParticipant.firstName;
    std::cout << "\nEnter the participant's last name: ";
    std::cin >> newParticipant.lastName;
    std::cout << "\nEnter " << newParticipant.firstName << " "
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

    participantFile.open("participant.dat");
    participantFile << newParticipant.firstName << " " 
                    << newParticipant.lastName << " "
                    << newParticipant.age << " "
                    << newParticipant.gender << " "
                    << newParticipant.id << std::endl;
    participantFile.close();
}

void collectSurvey() {
    std::cout << "Please choose the sstudy participant\n";
    for (int i = 0; i < participants.size(); i++) {
        std::cout << "ID: " << participants[i].id << "\n"
                  << "Name: " << participants[i].firstName << " " << participants[i].lastName << "\n"
                  << "Age: " << participants[i].age << "\n"
                  << "Gender: " << participants[i].gender << "\n"
                  << "StudyMor: " << participants[i].takesStudyMor();
        std::cout << std::endl 
                  << "**************************************************\n";
    }
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
            addNewParticipant();
            break;
        case 2:
            collectSurvey();
            break;
        case 3:
            //displayParticipant();
            break;
        case 4:
            //quit();
            break;
    }
}
