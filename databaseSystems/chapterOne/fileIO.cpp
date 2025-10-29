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
    int participantID;
    bool studyMor;
    std::string takesStudyMor() {
        if (participantID % 2 == 0) {
            studyMor = true;
            return "Yes\n";
        } else {
            studyMor = false;
            return "No\n";
        }
    }
};

struct survey {
    int surveyID;
    std::string headaches;
    std::string constipation;
    std::string difficultySleeping;
    std::string sideEffects;
    std::string canStudyMor;
};

std::vector<participant> participants;
std::vector<survey> surveys;

void menu();
void addNewParticipant();
void collectSurvey();
void displayParticipants(); 
void quit();

int main() {
    std::cout << "Welcome to the Westlake Research Hospital StudyMor Study!\n" << std::endl;
    menu();
}

void addNewParticipant() {
    participant newParticipant;
    int newID = 1;
    newID += participants.size();
    newParticipant.participantID = newID; 
    std::cout << "\nEnter the participant's first name: ";
    std::cin >> newParticipant.firstName;
    std::cout << "Enter the participant's last name: ";
    std::cin >> newParticipant.lastName;
    std::cout << "Enter " << newParticipant.firstName << " "
              << newParticipant.lastName << "'s age: ";
    std::cin >> newParticipant.age;
    std::cin.ignore(1000, '\n');
    std::cout << "Enter " << newParticipant.firstName << " "
              << newParticipant.lastName << "'s gender: ";
    std::cin >> newParticipant.gender;
    std::cout << "\n" << std::endl;
  
    participants.push_back(newParticipant);

    participantFile.open("participant.dat");
    participantFile << newParticipant.firstName << " " 
                    << newParticipant.lastName << " "
                    << newParticipant.age << " "
                    << newParticipant.gender << " "
                    << newParticipant.participantID << std::endl;
    participantFile.close();
}

void collectSurvey() {
    survey newSurvey;
    int idChoice;
    std::cout << "Please choose the sstudy participant\n";
    for (int i = 0; i < participants.size(); i++) {
        std::cout << "ID: " << participants[i].participantID << "\n"
                  << "Name: " << participants[i].firstName << " " << participants[i].lastName << "\n"
                  << "Age: " << participants[i].age << "\n"
                  << "Gender: " << participants[i].gender << "\n"
                  << "StudyMor: " << participants[i].takesStudyMor();
        std::cout << std::endl 
                  << "**************************************************\n";
    }

    std::cin >> idChoice;

    std::cout << "Ask the participant the following questions and enter their responses\n";
    
    std::cin.ignore(100000, '\n');

    std::cout << "1. Did you have any headaches using StudyMor? ";
    std::getline(std::cin, newSurvey.headaches);

    std::cout << "2. Did you have any constipation using StudyMor? ";
    std::getline(std::cin, newSurvey.constipation);

    std::cout << "3. Did you experience any difficulty sleeping while using StudyMor? ";
    std::getline(std::cin, newSurvey.difficultySleeping);

    std::cout << "4. List any other potential side effects, you experienced using StudyMor. ";
    std::getline(std::cin, newSurvey.sideEffects);

    std::cout << "5. Did you feel like you could study more using StudyMor? ";
    std::getline(std::cin, newSurvey.canStudyMor);

    std::cout << "\n" << std::endl;

    surveys.push_back(newSurvey);

    surveyFile.open("survey.dat");

    surveyFile << "Headache response: " << newSurvey.headaches << std::endl
               << "Constipation response: " << newSurvey.constipation << std::endl
               << "Difficulty sleeping response: " << newSurvey.difficultySleeping << std::endl
               << "Side effects response: " << newSurvey.sideEffects << std::endl
               << "Able to study more repsonse: " << newSurvey.canStudyMor << std::endl;
    surveyFile.close();
}

void displayParticipants() {
    for (int i = 0; i < participants.size(); i++) {
        std::cout << "ID: " << participants[i].participantID << "\n"
                  << "Name: " << participants[i].firstName << " " << participants[i].lastName << "\n"
                  << "Age: " << participants[i].age << "\n"
                  << "Gender: " << participants[i].gender << "\n"
                  << "StudyMor: " << participants[i].takesStudyMor();
        std::cout << std::endl 
                  << "**************************************************\n";
    }

    std::cout << "\n" << std::endl;
}

void menu() {
    int choice = 0;
    while (choice != 4) {
        std::cout << "1. Add a New Participant\n"
                  << "2. Collect Survey for Participant\n"
                  << "3. Display Participants\n"
                  << "4. Quit\n"
                  << std::endl
                  << "Please enter a command to continue: ";
        std::cin >> choice;
        std::cin.clear();
        switch (choice) {
            case 1:
                addNewParticipant();
                break;
            case 2:
                collectSurvey();
                break;
            case 3:
                displayParticipants();
                break;
            case 4:
                std::cout << "Exiting program";
                break;
            default:
                std::cout << "Try again";
                break;
        }
    }
}