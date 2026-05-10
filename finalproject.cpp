/* 
Boses ng Barangay: Concern Reporting & Monitoring System

This is a console-based program (C++) that provides barangay residents and leaders an organized process of reporting, monitoring, and resolving concerns
arising within the community. 

The system must allow users (residents) to report their concern with all needed details, provide users (leaders / admin) password-protected 
access to view all concerns and update their status, identify overdue concerns and urgent issues, and allow users (residents) to monitor 
submitted reports and their status by entering their name.
*/

// Final Project (Console-based) - CC 103: Data Structures and Algorithms

// Header Files - declarations
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>
using namespace std;

// struct definition - storing all information related to the report - combine the information (report details) into one record
struct Concern {
    int id;
    string name;
    string location;
    string description;
    string category;
    string urgencyLevel;
    string status;
    string date;
};

/* vector - store multiple concern records dynamically
load_concerns() – reads all records from CSV, returning them as a list of dictionaries
*/
vector<Concern> loadConcerns() {
    vector<Concern> concerns;
    ifstream myFile("concernreports.txt");
    if (!myFile) {
        return concerns;
    }

    string line;
    while(getline(myFile, line)) {
        stringstream ss(line);
        Concern c;

        getline(ss, line, ','); c.id = stoi(line);
        getline(ss, c.name, ',');
        getline(ss, c.location, ',');
        getline(ss, c.description, ',');
        getline(ss, c.category, ',');
        getline(ss, c.urgencyLevel, ',');
        getline(ss, c.status, ',');
        getline(ss, c.date, ',');

        concerns.push_back(c); // add new data (concern report) at the end of a vector
    }
    myFile.close();
    return concerns;
}

// Writes the updated data back to the txt file
void saveConcerns(vector<Concern> &concerns) {
    ofstream myFile("concernreports.txt");

    for (auto &c : concerns) {
        myFile << c.id << "," << c.name << "," << c.location << "," << c.description << "," << c.category << "," << c.urgencyLevel << "," << c.status << "," << c.date << "\n";
    }
    
    myFile.close();
}

// To get the submission date of the report, to convert string date into time format, and to calculate how many days since the report date
string getCurrentDate () {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    return to_string(1900 + ltm->tm_year) + "-" +
           to_string(1 + ltm->tm_mon) + "-" +
           to_string(ltm->tm_mday);
}

time_t stringToDate(string dateStr) {
    tm t = {};
    sscanf(dateStr.c_str(), "%d-%d-%d",
           &t.tm_year, &t.tm_mon, &t.tm_mday);

    t.tm_year -= 1900;
    t.tm_mon -= 1;

    return mktime(&t);
}

int daysSince(string dateStr) {
    time_t now = time(0);
    time_t reported = stringToDate(dateStr);

    double seconds = difftime(now, reported);
    return seconds / (60 * 60 * 24);
}

// Converts string into lowercase format
string toLower(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// To check if concern is overdue, critical, high, or resolved
bool isOverdue(Concern c) {
    return (c.status == "Pending" || c.status == "In Progress") && daysSince(c.date) >= 5;
}

bool isCritical(Concern c) {
    return (c.status == "Pending" || c.status == "In Progress") && c.urgencyLevel == "Critical";
}

bool isHigh(Concern c) {
    return (c.status == "Pending" || c.status == "In Progress") && c.urgencyLevel == "High";
}

bool isResolved(Concern c) {
    return c.status == "Resolved";
}

// Function declaration
void residentMenu();
void submitConcern();
void viewMyConcerns();
void adminMenu();
void viewAllConcerns();
void updateStatus();

int main(){
    int choice;

    // Main menu - first to be displayed
    do{ 
        cout << "\n=== Boses ng Barangay: Concern Reporting & Reporting System ===\n\n";
        cout << "Every crack left unfilled eventually brings the whole wall down.\n\n";
        cout << "Choose your role (1/2/3):" << endl;
        cout << "1. Resident" << endl;
        cout << "2. Admin (Brgy. Official)" << endl;
        cout << "3. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;
        
        if (choice == 1) {
            residentMenu();
        } else if (choice == 2) {
            string password;

            cout << "\nAccess is granted to barangay officials only.\n";
            cout << "Enter password to continue: ";
            cin >> password;

            if (password == "brgyofficial") {
                adminMenu();
            } else {
                cout << "Access Denied\n";
            }
        }
    } while (choice != 3);  // keeps the menu running until option 3 is chosen (3 - exit the program)

    return 0;
}

// Function Definition

//  Displays a sub-menu that routes to the chosen resident feature
void residentMenu() {
    int feature;

    do{
        cout << "\n--- Resident Menu ---\n\n";
        cout << "Choose according to your desired feature (1/2/3): \n";
        cout << "1. Report a Concern" << endl;
        cout << "2. View My Concerns" << endl;
        cout << "3. Back to Main Menu" << endl;
        cout << "Feature: ";
        cin >> feature;

        if (feature == 1) {
            submitConcern();
        } else if (feature == 2) {
            viewMyConcerns();
        }
    } while(feature != 3);

}

// Launches a form that collects user (resident) input; appends the report to the txt file with a timestamp and default status 
void submitConcern() {
    vector<Concern> concerns = loadConcerns();
    Concern c;

    cout << "\n--- Report Form ---\n";
    cout << "Please fill out all necessary details.\n\n";

    c.id = concerns.size() + 1; // assigns id to the report
    cin.ignore();

    cout << "Name: ";
    getline(cin, c.name);
    cout << "Purok/Street: ";
    getline(cin, c.location);
    cout << "Description: ";
    getline(cin, c.description);
    cout << "Category (Infrastructure/Safety/Environment/Noise Complaint/Health/Others): ";
    getline(cin, c.category);
    cout << "Urgency Level (Low/Medium/High/Critical): ";
    getline(cin, c.urgencyLevel);
    c.status = "Pending";   // to assign the status automatically
    c.date = getCurrentDate();  // program automatically identifies the date the concern was submitted

    concerns.push_back(c);
    saveConcerns(concerns);

    cout << "\nYour report has been submitted. You may review the details on 'View My Concerns' feature. The barangay officials will act promptly.\n";
}

// Iterates through the records to find, and display records matching the entered name
void viewMyConcerns() {
    vector<Concern> concerns = loadConcerns();
    string name;

    cout << "\n--- View My Concerns ---\n";
    cout << "Records matching your name will be displayed.\n\n";

    cin.ignore();
    cout << "Enter your name: ";
    getline(cin, name);

    cout << "\nResults matching \'" << name << "\':"; 

    bool found = false;
    for (auto &c : concerns) {
        if (toLower(c.name) == toLower(name)) {
            found = true;
            cout << "\n--------------------------\n";
            
            if (isOverdue(c)) {
                cout << "!OVERDUE! (" << to_string(daysSince(c.date)) << " days)\n";    // "!text! are like tags or labels in order to emphasize that the concern is overdue / urgent"
            }
            if (isCritical(c)) {
                cout << "!CRITICAL!\n";
            } else if (isHigh(c)) {
                cout << "!HIGH!\n";
            } else if (isResolved(c)) {
                cout << "!RESOLVED!\n";
            }

            cout << "ID: " << c.id 
                << "\nLocation: " << c.location << "\nDescription: " << c.description << "\nCategory: " << c.category << "\nUrgency Level: "
                << c.urgencyLevel << "\nStatus: " << c.status << "\nDate Reported: " << c.date << "\n--------------------------\n";
        }
    }

    if (!found) {
        cout << "\nNo concerns found under \'" << name << "\'.\n";
    }
}

// Displays a sub-menu (password-protected access) that routes to the chosen admin feature 
void adminMenu() {
    int feature;

    do{
        cout << "\n--- Welcome, Admin! ---\n\n";
        cout << "Choose according to your desired feature (1/2/3): \n";
        cout << "1. View All Concerns" << endl;
        cout << "2. Update Status" << endl;
        cout << "3. Back to Main Menu" << endl;
        cout << "Feature: ";
        cin >> feature;

        if (feature == 1) {
            viewAllConcerns();
        } else if (feature == 2) {
            updateStatus();
        }
    } while(feature != 3);
}

// Displays in-memory record of all concerns, providing two sections (Active and Resolved) alongside the total counts by status 
void viewAllConcerns() {
    vector<Concern> concerns = loadConcerns();

    cout << "\n--- View All Concerns ---\n";
    cout << "This is the list of all the concerns submitted by the residents.\n";

    // Must display first the total number of reports submitted, and number of reports per status
    int totalCount = concerns.size();
    int pendingC = 0;
    int inProgressC = 0;
    int resolvedC = 0;

    for (auto &c : concerns) {
        if (c.status == "Pending") {
            pendingC++;
        } else if (c.status == "In Progress") {
            inProgressC++;
        } else if (c.status == "Resolved") {
            resolvedC++;
        }
    }

    cout << "\n[[[ Total Count of Reported Concerns ]]]\n";
    cout << "\nTotal: " << totalCount;
    cout << "\nPending: " << pendingC;
    cout << "\nIn Progress: " << inProgressC;
    cout << "\nResolved: " << resolvedC;

    cout << "\n\n--------------------------\n";
    cout << "\n[[[ Active Concerns ]]]\n";  // first section for unresolved concerns
    bool foundActive = false;

    for (auto &c : concerns) {
        if (c.status == "Pending" || c.status == "In Progress") {
            foundActive = true;
            cout << "\n--------------------------\n";

            if (isOverdue(c)) {
                cout << "!OVERDUE! (" << to_string(daysSince(c.date)) << " days)\n";    // "!text! are like tags or labels in order to highlight that the concern is overdue / urgent"
            }
            if (isCritical(c)) {
                cout << "!CRITICAL!\n";
            } else if (isHigh(c)) {
                cout << "!HIGH!\n";
            }

            cout << "ID: " << c.id << "\nName: " << c.name << "\nLocation: " << c.location << "\nDescription: " << c.description 
                 << "\nCategory: " << c.category << "\nUrgency Level: " << c.urgencyLevel << "\nStatus: " << c.status << "\nDate: " << c.date
                 << "\n--------------------------\n";
        }
    }
    if (!foundActive) {
        cout << "\nNo reported concerns yet.\n";
    }

    cout << "\n[[[ Resolved Concerns ]]]\n";    // second section for resolved concerns
    bool foundResolved = false;

    for (auto &c : concerns) {    
        if (isResolved(c)) {
            foundResolved = true;

            cout << "\n--------------------------\n";
            cout << "!RESOLVED!\n";
            cout << "ID: " << c.id << "\nName: " << c.name << "\nLocation: " << c.location << "\nDescription: " << c.description 
                 << "\nCategory: " << c.category << "\nUrgency Level: " << c.urgencyLevel << "\nStatus: " << c.status << "\nDate: " << c.date
                 << "\n--------------------------\n";
        }
    }

    if (!foundResolved) {
        cout << "\nNo resolved concerns yet.\n";
    }
}

// Displays all (unresolved) concerns; admin is prompted to select a concern to update 
void updateStatus() {
    vector<Concern> concerns = loadConcerns();

    cout << "\n--- Update Status ---\n";
    cout << "Please choose a concern to update.\n";

    cout << "\n[[[ Concerns to Update ]]]\n";   // must display first the list of concerns so the admin can see which needs update
    bool foundUnresolved = false;

    for (auto &c : concerns) {
        if (c.status == "Pending" || c.status == "In Progress") {
            foundUnresolved = true;
            cout << "\n--------------------------\n";

            if (isOverdue(c)) {
                cout << "!OVERDUE! (" << to_string(daysSince(c.date)) << " days)\n";
            }
            if (isCritical(c)) {
                cout << "!CRITICAL!\n";
            } else if (isHigh(c)) {
                cout << "!HIGH!\n";
            }

            cout << "ID: " << c.id << "\nName: " << c.name << "\nLocation: " << c.location << "\nDescription: " << c.description 
                 << "\nCategory: " << c.category << "\nUrgency Level: " << c.urgencyLevel << "\nStatus: " << c.status << "\nDate: " << c.date
                 << "\n--------------------------\n";
        }
    }
    if (!foundUnresolved) {
        cout << "\nNo concerns to update yet.\n";
        return;
    }

    int id;
    cout << "\nEnter ID (e.g. 1) to update: ";  // admin only needs to enter the id
    cin >> id;

    cin.ignore();
    string newStatus;

    cout << "New Status (In Progress / Resolved): ";    // only two options for the new status
    getline(cin, newStatus);

    for (auto &c : concerns) {
        if (c.id == id) {
            c.status = newStatus;   // update current status to the new status
        }
    }

    saveConcerns(concerns);

    cout << "\nConcern updated successfully!\n";
}