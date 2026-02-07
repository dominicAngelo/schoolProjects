/* Program name: fileIO.cpp
*  Author: Dominic Angelo
*  Date last updated 10/27/25
*  Purpose: Hospital StudyMor Study
*/

#include <fstream>
#include <iostream>
#include <vector>

//describes all of the data for our participant
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
            return "Yes";
        } else {
            studyMor = false;
            return "No";
        }
    }
};

//the data necessary for our survey
struct survey { 
    int surveyID;
    std::string headaches;
    std::string constipation;
    std::string difficultySleeping;
    std::string sideEffects;
    std::string canStudyMor;
};

std::vector<participant> participants; // these vectors are used to display the participant data
std::vector<survey> surveys;

void menu();
void loadData();
void addNewParticipant();
void collectSurvey();
void displayParticipants(); 
void quit();

int main() {
    std::cout << "Welcome to the Westlake Research Hospital StudyMor Study!\n" << std::endl;
    loadData();
    menu();
}

// function will load data from the .dat files intot the vectors
void loadData() {
    participants.clear();
    surveys.clear(); // clears the vectors from old data to prevent duplication
    participant loadParticipant;
    std::ifstream participantInput("participant.dat");
    if (participantInput.is_open()) {
        while (participantInput >> loadParticipant.firstName
                                >> loadParticipant.lastName
                                >> loadParticipant.age
                                >> loadParticipant.gender
                                >> loadParticipant.participantID) 
        {
            loadParticipant.takesStudyMor();
            participants.push_back(loadParticipant);
        }
        participantInput.close();
    } else {
        std::cerr << "Error: Could not open participant.dat\n";
    }

    survey loadSurvey;
    std::ifstream surveyInput("survey.dat");
    if (surveyInput.is_open()) {
        while (surveyInput >> loadSurvey.surveyID
                           >> loadSurvey.headaches
                           >> loadSurvey.constipation
                           >> loadSurvey.difficultySleeping
                           >> loadSurvey.sideEffects
                           >> loadSurvey.canStudyMor) 
        {
            surveys.push_back(loadSurvey);
        }
        surveyInput.close();
    } else {
        std::cerr << "Error: Could not open survey.dat\n";
    }
}

// function to add a new participant
void addNewParticipant() {
    participant newParticipant;
    int newID = 0;
    if (participants.empty()) {
        newID = 1; 
    } else {
        newID = participants.back().participantID + 1;
    }
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
  
    participants.push_back(newParticipant); //pushes the participant data into the vector.

    // writes the participant file out to the .dat file
    std::ofstream participantFile("participant.dat", std::ios::app);
    if (participantFile.is_open()) { // checks to see if file is open safely
        participantFile << newParticipant.firstName << " " 
                        << newParticipant.lastName << " "
                        << newParticipant.age << " "
                        << newParticipant.gender << " "
                        << newParticipant.participantID << std::endl;
        participantFile.close();
    } else {
        std::cerr << "Error: Could not open participant.dat\n";
    }
}

//function to collect survey data
void collectSurvey() {
    survey newSurvey;
    int idChoice;
    std::cout << "Please choose the study participant\n";
    for (int i = 0; i < participants.size(); i++) {
        std::cout << "ID: " << participants[i].participantID << "\n"
                  << "Name: " << participants[i].firstName << " " << participants[i].lastName << "\n"
                  << "Age: " << participants[i].age << "\n"
                  << "Gender: " << participants[i].gender << "\n"
                  << "StudyMor: " << participants[i].takesStudyMor();
        std::cout << std::endl 
                  << "**************************************************\n";
    }
    /* the id is the simplest way to match the survey with the participant, 
     *when you enter the idchoice you are entering the chosen participant,
     *the survey is assigned the input id*/
    std::cin >> idChoice;
    newSurvey.surveyID = idChoice;

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

    surveys.push_back(newSurvey); // writes survey data to the vector (this is an unnecessary behavior)

    // writes survey data to .dat file
    std::ofstream surveyFile("survey.dat", std::ios::app);
<<<<<<< HEAD

    if (!(surveyFile.is_open())) {
        std::cout << "Error! 'survey.dat' could not be opened or does not exist.";
    }

    surveyFile << "Headache response: " << newSurvey.headaches << std::endl
               << "Constipation response: " << newSurvey.constipation << std::endl
               << "Difficulty sleeping response: " << newSurvey.difficultySleeping << std::endl
               << "Side effects response: " << newSurvey.sideEffects << std::endl
               << "Able to study more repsonse: " << newSurvey.canStudyMor << std::endl;
    surveyFile.close();
=======
    if (surveyFile.is_open()) { // checks to make sure file is open safely
        surveyFile << newSurvey.surveyID << " "
                   << newSurvey.headaches << " "
                   << newSurvey.constipation << " "
                   << newSurvey.difficultySleeping << " "
                   << newSurvey.sideEffects << " "
                   << newSurvey.canStudyMor << std::endl;
        surveyFile.close();
    } else {
        std::cerr << "Error: Could not open survey.dat\n";
    }
>>>>>>> 01903dc05b3b4f9bbcfe470bae4dbfbcb218fce4
}

void displayParticipants() { // uses the participants vector to output all participant data in a loop
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
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
        }
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
<<<<<<< HEAD
=======
                std::exit(EXIT_SUCCESS); //exits the program
>>>>>>> 01903dc05b3b4f9bbcfe470bae4dbfbcb218fce4
                break;
            default:
                std::cout << "Input not valid. Please try again";
                break;
        }
    }
}